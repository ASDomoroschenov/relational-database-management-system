#include "handle_request_concrete.h"
#include <functional>

//begin region handler_concrete

//DESCRIPTION
//SURNAME
//NAME
//PATRONYMIC
//MAIL
//PHONE
//COMMENT
//DATETIME

handler_concrete::handler_concrete()
{
    handler *_run = new handler_run;
    handler *_add_pool = new handler_add_pool;
    handler *_remove_pool = new handler_remove_pool;
    handler *_add_scheme = new handler_add_scheme;
    handler *_remove_scheme = new handler_remove_scheme;
    handler *_add_collection = new handler_add_collection;
    handler *_remove_collection = new handler_remove_collection;
    handler *_add = new handler_add;
    handler *_get = new handler_get;
    handler *_get_range = new handler_get_range;
    handler *_update = new handler_update;
    handler *_remove = new handler_remove;

    set_next(_run);
    _run->set_next(_add_pool);
    _add_pool->set_next(_remove_pool);
    _remove_pool->set_next(_add_scheme);
    _add_scheme->set_next(_remove_scheme);
    _remove_scheme->set_next(_add_collection);
    _add_collection->set_next(_remove_collection);
    _remove_collection->set_next(_add);
    _add->set_next(_get);
    _get->set_next(_get_range);
    _get_range->set_next(_update);
    _update->set_next(_remove);
    _remove->set_next(nullptr);
}

void handler_concrete::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    try
    {
        _next_handler->handle_request(params, stream, console);
    }
    catch (const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

handler_concrete::~handler_concrete()
{
    handler::get_logger()->log("Deleting handler", logger::severity::INFORMATION);

    auto *data_base = reinterpret_cast<type_data_base_rb*>(handler::get_instance());
    auto data_base_end = data_base->end_prefix();

    for (auto it_base = data_base->begin_prefix(); it_base != data_base_end; ++it_base)
    {
        auto end_pool= std::get<2>(*it_base).first->end_prefix();
        auto begin_pool = std::get<2>(*it_base).first->begin_prefix();

        for (auto it_pool = begin_pool; it_pool != end_pool; ++it_pool)
        {
            auto end_scheme = std::get<2>(*it_pool).first->end_prefix();
            auto begin_scheme = std::get<2>(*it_pool).first->begin_prefix();

            for (auto it_scheme = begin_scheme; it_scheme != end_scheme; ++it_scheme)
            {
                auto end_collection = std::get<2>(*it_scheme)->first->end_prefix();
                auto begin_collection = std::get<2>(*it_scheme)->first->begin_prefix();

                for (auto it_collection = begin_collection; it_collection != end_collection; ++it_collection)
                {
                    delete std::get<2>(*it_collection)->first;
                    delete std::get<2>(*it_collection);
                }

                delete std::get<2>(*it_scheme)->first;
                delete std::get<2>(*it_scheme)->second["DESCRIPTION"];
                delete std::get<2>(*it_scheme)->second["SURNAME"];
                delete std::get<2>(*it_scheme)->second["NAME"];
                delete std::get<2>(*it_scheme)->second["PATRONYMIC"];
                delete std::get<2>(*it_scheme)->second["MAIL"];
                delete std::get<2>(*it_scheme)->second["PHONE"];
                delete std::get<2>(*it_scheme)->second["COMMENT"];
                delete std::get<2>(*it_scheme)->second["DATETIME"];
                delete std::get<2>(*it_scheme);
                std::get<2>(*it_pool).first->remove(std::get<1>(*it_scheme));
            }

            delete std::get<2>(*it_pool).first;
            std::get<2>(*it_base).first->remove(std::get<1>(*it_pool));
        }

        delete std::get<2>(*it_base).second;
        delete std::get<2>(*it_base).first;
        data_base->remove(std::get<1>(*it_base));
    }

    delete handler::get_instance();

    handler::get_logger()->log("Handler is deleted", logger::severity::INFORMATION);
}

//end region handler_concrete


//begin region handler_run

void handler_run::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{

    if (!params.empty() && params[0] == "RUN")
    {
        handler::get_logger()->log("RUN command called", logger::severity::INFORMATION);

        command *execute_run = new run_command(this);

        try
        {
            execute_run->execute(params, nullptr, nullptr, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_run;
            throw std::invalid_argument(ex.what());
        }

        delete execute_run;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

handler_run::run_command::run_command(
        handler *handler_init) :
        _handler(handler_init)
{

}

void handler_run::run_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    std::fstream stream_file(params[1]);
    std::string line;

    if (!stream_file.is_open())
    {
        throw std::invalid_argument("ERROR: Invalid file to run commands");
    }

    while (std::getline(stream_file, line))
    {
        std::vector<std::string> params_file_command = split_by_spaces(delete_spaces(line));

        try
        {
            _handler->handle_request(params_file_command, stream_file, false);
        }
        catch (const std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }
    }

    stream_file.close();
}

//end region handler_run


//begin region handler_add_pool

void handler_add_pool::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "ADD_POOL")
    {
        handler::get_logger()->log("ADD_POOL command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 2)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add pool");
        }

        if (data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Pool with the same name already exists");
        }

        command *execute_add_pool = new add_pool_command;

        try
        {
            execute_add_pool->execute(params, nullptr, nullptr, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_add_pool;
            throw std::invalid_argument(ex.what());
        }

        delete execute_add_pool;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_add_pool::add_pool_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    type_data_base *data_base = handler::get_instance();

    if (console)
    {
        int number_allocator = 0;

        std::cout << "Choose an allocator:" << std::endl;
        std::cout << "1. Allocator list" << std::endl;
        std::cout << "2. Allocator descriptor" << std::endl;
        std::cout << "3. Allocator buddies" << std::endl;
        std::cout << "Put the number: ";
        std::cin >> number_allocator;

        switch (number_allocator)
        {
            case 1:
                data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_list(SIZE_TO_ALLOCATOR)));
                break;
            case 2:
                data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_descriptor(SIZE_TO_ALLOCATOR)));
                break;
            case 3:
                data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_buddies(SIZE_TO_ALLOCATOR)));
                break;
            default:
                throw std::invalid_argument("ERROR: Invalid number of allocator");
        }
    }
    else
    {
        std::string allocator_params;

        if (stream.eof())
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::out_of_range("ERROR: Unable to read allocator name");
        }

        std::getline(stream, allocator_params);
        std::string name_allocator;

        try
        {
            read_arguments(allocator_params, "ALLOCATOR", ":", name_allocator);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument(ex.what());
        }

        if (name_allocator == "allocator_list")
        {
            data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_list(SIZE_TO_ALLOCATOR)));
        }
        else if (name_allocator == "allocator_descriptor")
        {
            data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_descriptor(SIZE_TO_ALLOCATOR)));
        }
        else if (name_allocator == "allocator_buddies")
        {
            data_base->insert(params[1], std::make_pair(handler::allocate_pool(), new allocator_buddies(SIZE_TO_ALLOCATOR)));
        }
        else
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid name of allocator");
        }
    }


}

//end region handler_add_pool


//begin region handler_remove_pool

void handler_remove_pool::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "REMOVE_POOL")
    {
        handler::get_logger()->log("REMOVE_POOL command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 2)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to remove pool");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        command *execute_remove_pool = new remove_pool_command();

        try
        {
            execute_remove_pool->execute(params, &pool, nullptr, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_remove_pool;
            throw std::invalid_argument(ex.what());
        }

        delete execute_remove_pool;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_remove_pool::remove_pool_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    auto end_pool = reinterpret_cast<type_pool_rb*>(pool->first)->end_prefix();
    auto begin_pool = reinterpret_cast<type_pool_rb*>(pool->first)->end_prefix();

    for (auto it_pool = begin_pool; it_pool != end_pool; ++it_pool)
    {
        auto end_scheme = std::get<2>(*it_pool).first->end_prefix();
        auto begin_scheme = std::get<2>(*it_pool).first->begin_prefix();

        for (auto it_scheme = begin_scheme; it_scheme != end_scheme; ++it_scheme)
        {
            auto end_collection = std::get<2>(*it_scheme)->first->end_prefix();
            auto begin_collection = std::get<2>(*it_scheme)->first->begin_prefix();

            for (auto it_collection = begin_collection; it_collection != end_collection; ++it_collection)
            {
                delete std::get<2>(*it_collection)->first;
                delete std::get<2>(*it_collection);
            }

            delete std::get<2>(*it_scheme)->first;
            delete std::get<2>(*it_scheme)->second["DESCRIPTION"];
            delete std::get<2>(*it_scheme)->second["SURNAME"];
            delete std::get<2>(*it_scheme)->second["NAME"];
            delete std::get<2>(*it_scheme)->second["PATRONYMIC"];
            delete std::get<2>(*it_scheme)->second["MAIL"];
            delete std::get<2>(*it_scheme)->second["PHONE"];
            delete std::get<2>(*it_scheme)->second["COMMENT"];
            delete std::get<2>(*it_scheme)->second["DATETIME"];
            delete std::get<2>(*it_scheme);
            std::get<2>(*it_pool).first->remove(std::get<1>(*it_scheme));
        }

        pool->first->remove(std::get<1>(*it_pool));
        delete std::get<2>(*it_pool).first;
    }

    delete pool->first;
    delete pool->second;
    handler::get_instance()->remove(params[1]);

    pool->first = nullptr;
    pool->second = nullptr;
}

//end region handler_remove_pool


//begin region handler_add_scheme

void handler_add_scheme::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "ADD_SCHEME")
    {
        handler::get_logger()->log("ADD_SCHEME command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 3)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add data_scheme");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("Scheme with the same name already exists");
        }

        command *execute_add_scheme = new add_scheme_command;

        try
        {
            execute_add_scheme->execute(params, &pool, nullptr, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_add_scheme;
            throw std::invalid_argument(ex.what());
        }

        delete execute_add_scheme;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_add_scheme::add_scheme_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    (*pool).first->insert(params[2], std::make_pair(handler::allocate_scheme(), (*pool).second));
}

//end region handler_add_scheme


//begin region handler_remove_scheme

void handler_remove_scheme::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "REMOVE_SCHEME")
    {
        handler::get_logger()->log("REMOVE_SCHEME command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 3)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to remove data_scheme");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        command *execute_remove_scheme = new remove_scheme_command;

        try
        {
            execute_remove_scheme->execute(params, &pool, &scheme, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_remove_scheme;
            throw std::invalid_argument(ex.what());
        }

        delete execute_remove_scheme;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_remove_scheme::remove_scheme_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    auto end_scheme = reinterpret_cast<type_scheme_rb*>(scheme->first)->end_prefix();
    auto begin_scheme = reinterpret_cast<type_scheme_rb*>(scheme->first)->begin_prefix();

    for (auto it_scheme = begin_scheme; it_scheme != end_scheme; ++it_scheme)
    {
        auto end_collection = std::get<2>(*it_scheme)->first->end_prefix();
        auto begin_collection = std::get<2>(*it_scheme)->first->begin_prefix();

        for (auto it_collection = begin_collection; it_collection != end_collection; ++it_collection)
        {
            delete std::get<2>(*it_collection)->first;
            delete std::get<2>(*it_collection);
        }

        delete std::get<2>(*it_scheme)->first;
        delete std::get<2>(*it_scheme)->second["DESCRIPTION"];
        delete std::get<2>(*it_scheme)->second["SURNAME"];
        delete std::get<2>(*it_scheme)->second["NAME"];
        delete std::get<2>(*it_scheme)->second["PATRONYMIC"];
        delete std::get<2>(*it_scheme)->second["MAIL"];
        delete std::get<2>(*it_scheme)->second["PHONE"];
        delete std::get<2>(*it_scheme)->second["COMMENT"];
        delete std::get<2>(*it_scheme)->second["DATETIME"];
        delete std::get<2>(*it_scheme);
        scheme->first->remove(std::get<1>(*it_scheme));
    }

    delete scheme->first;
    pool->first->remove(params[2]);

    scheme->first = nullptr;
}

//end region handler_remove_scheme


//begin region handler_add_collection

void handler_add_collection::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "ADD_COLLECTION")
    {
        handler::get_logger()->log("ADD_COLLECTION command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("Collection with the same name already exists");
        }

        command *execute_add_collection = new add_collection_command;

        try
        {
            execute_add_collection->execute(params, &pool, &scheme, nullptr, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_add_collection;
            throw std::invalid_argument(ex.what());
        }

        delete execute_add_collection;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_add_collection::add_collection_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    type_data_base *data_base = handler::get_instance();

    std::map<std::string, type_order_collection*> order_collections;
    order_collections.insert({"DESCRIPTION", new type_order_collection()});
    order_collections.insert({"SURNAME", new type_order_collection()});
    order_collections.insert({"NAME", new type_order_collection()});
    order_collections.insert({"PATRONYMIC", new type_order_collection()});
    order_collections.insert({"MAIL", new type_order_collection()});
    order_collections.insert({"PHONE", new type_order_collection()});
    order_collections.insert({"COMMENT", new type_order_collection()});
    order_collections.insert({"DATETIME", new type_order_collection()});

    if (console)
    {
        int number_tree = 0;

        std::cout << "Choose an type of tree for collection:" << std::endl;
        std::cout << "1. Red-black tree" << std::endl;
        std::cout << "2. Splay tree" << std::endl;
        std::cout << "3. AVL tree" << std::endl;
        std::cout << "Put the number: ";
        std::cin >> number_tree;

        try
        {
            scheme->first->insert(params[3], new std::pair<type_collection*, std::map<std::string, type_order_collection*>>(std::make_pair(handler::allocate_collection(pool->second, static_cast<handler::TREE>(number_tree - 1)), order_collections)));
        }
        catch (const std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }
    }
    else
    {
        std::string tree_params;

        if (stream.eof())
        {
            throw std::out_of_range("ERROR: Unable to read type tree");
        }

        std::getline(stream, tree_params);
        std::string name_type;

        try
        {
            read_arguments(tree_params, "TREE", ":", name_type);
        }
        catch (const std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }

        if (name_type == "RED_BLACK_TREE")
        {
            scheme->first->insert(params[3], new std::pair<type_collection*, std::map<std::string, type_order_collection*>>(std::make_pair(handler::allocate_collection(pool->second, handler::TREE::RED_BLACK_TREE), order_collections)));
        }
        else if (name_type == "AVL_TREE")
        {
            scheme->first->insert(params[3], new std::pair<type_collection*, std::map<std::string, type_order_collection*>>(std::make_pair(handler::allocate_collection(pool->second, handler::TREE::AVL_TREE), order_collections)));
        }
        else if (name_type == "SPLAY_TREE")
        {
            scheme->first->insert(params[3], new std::pair<type_collection*, std::map<std::string, type_order_collection*>>(std::make_pair(handler::allocate_collection(pool->second, handler::TREE::SPLAY_TREE), order_collections)));
        }
        else
        {
            throw std::invalid_argument("ERROR: Invalid name of type tree");
        }
    }
}

//end region handler_add_collection


//begin region handler_remove_collection

void handler_remove_collection::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "REMOVE_COLLECTION")
    {
        handler::get_logger()->log("REMOVE_COLLECTION command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to remove collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_remove_collection = new remove_collection_command;

        try
        {
            execute_remove_collection->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_remove_collection;
            throw std::invalid_argument(ex.what());
        }

        delete execute_remove_collection;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_remove_collection::remove_collection_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    auto end_collection = reinterpret_cast<type_collection_rb*>(scheme->first->get(params[3])->first)->end_prefix();
    auto begin_collection = reinterpret_cast<type_collection_rb*>((*scheme).first->get(params[3])->first)->begin_prefix();

    for (auto it_collection = begin_collection; it_collection != end_collection; ++it_collection)
    {
        delete std::get<2>(*it_collection)->first;
        delete std::get<2>(*it_collection);
    }

    delete collection->first;
    delete collection->second["DESCRIPTION"];
    delete collection->second["SURNAME"];
    delete collection->second["NAME"];
    delete collection->second["PATRONYMIC"];
    delete collection->second["MAIL"];
    delete collection->second["PHONE"];
    delete collection->second["COMMENT"];
    delete collection->second["DATETIME"];
    delete collection;

    scheme->first->remove(params[3]);

    collection = nullptr;
}

//end region handler_remove_collection


//begin region handler_add

void handler_add::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "ADD")
    {
        handler::get_logger()->log("ADD command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_add = new add_command;

        try
        {
            execute_add->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_add;
            throw std::invalid_argument(ex.what());
        }

        delete execute_add;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_add::add_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    size_t id_user = 0;
    size_t id_delivery = 0;
    delivery_data *to_insert = nullptr;

    try
    {
        if (console)
        {
            read_id_data_id_delivery_from_console(id_user, id_delivery);
            read_data_delivery_from_console(to_insert);

            if (collection->first->in(std::make_pair(id_user, id_delivery)))
            {
                throw std::invalid_argument("ERROR: The same delivery data already exists");
            }
        }
        else
        {
            read_id_data_id_delivery(id_user, id_delivery, stream);
            read_data_delivery(to_insert, stream);

            if (collection->first->in(std::make_pair(id_user, id_delivery)))
            {
                throw std::invalid_argument("ERROR: The same delivery data already exists");
            }
        }
    }
    catch (const std::exception &ex)
    {
        throw std::invalid_argument(ex.what());
    }

    auto *to_insert_value = new type_value(std::make_pair(to_insert, type_states()));
    collection->first->insert(std::make_pair(id_user, id_delivery), to_insert_value);
    collection->second["DESCRIPTION"]->insert(to_insert->get_description()->get_value(), to_insert_value);
    collection->second["SURNAME"]->insert(to_insert->get_surname()->get_value(), to_insert_value);
    collection->second["NAME"]->insert(to_insert->get_name()->get_value(), to_insert_value);
    collection->second["PATRONYMIC"]->insert(to_insert->get_patronymic()->get_value(), to_insert_value);
    collection->second["MAIL"]->insert(to_insert->get_mail()->get_value(), to_insert_value);
    collection->second["PHONE"]->insert(to_insert->get_phone()->get_value(), to_insert_value);
    collection->second["COMMENT"]->insert(to_insert->get_user_comment()->get_value(), to_insert_value);
    collection->second["DATETIME"]->insert(to_insert->get_date_time_delivery()->get_value(), to_insert_value);
}

//end region handler_add


//begin region handler_get

void handler_get::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "GET")
    {
        handler::get_logger()->log("GET command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_get = new get_command;

        try
        {
            execute_get->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_get;
            throw std::invalid_argument(ex.what());
        }

        delete execute_get;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

delivery_data *get_state(
        const std::string &datetime,
        type_value *states)
{
    auto *temp = new delivery_data();
    temp->set_description(states->first->get_description()->get_value());
    temp->set_surname(states->first->get_surname()->get_value());
    temp->set_name(states->first->get_name()->get_value());
    temp->set_patronymic(states->first->get_patronymic()->get_value());
    temp->set_mail(states->first->get_mail()->get_value());
    temp->set_phone(states->first->get_phone()->get_value());
    temp->set_user_comment(states->first->get_user_comment()->get_value());
    temp->set_date_time_delivery(states->first->get_date_time_delivery()->get_value());

    if (datetime.empty())
    {
        if (!states->second.empty())
        {
            for (const auto &item: states->second)
            {
                for (const auto &item_states: states->second[item.first])
                {
                    if (item_states.first == "DESCRIPTION")
                    {
                        temp->set_description(item_states.second);
                    }
                    else if (item_states.first == "SURNAME")
                    {
                        temp->set_surname(item_states.second);
                    }
                    else if (item_states.first == "NAME")
                    {
                        temp->set_name(item_states.second);
                    }
                    else if (item_states.first == "PATRONYMIC")
                    {
                        temp->set_patronymic(item_states.second);
                    }
                    else if (item_states.first == "MAIL")
                    {
                        temp->set_mail(item_states.second);
                    }
                    else if (item_states.first == "PHONE")
                    {
                        temp->set_phone(item_states.second);
                    }
                    else if (item_states.first == "COMMENT")
                    {
                        temp->set_user_comment(item_states.second);
                    }
                    else if (item_states.first == "DATETIME")
                    {
                        temp->set_date_time_delivery(item_states.second);
                    }
                }
            }
        }

        return temp;
    }

    long long time = get_time(datetime);

    if (!states->second.empty())
    {
        for (const auto &item: states->second)
        {
            if (time > item.first)
            {
                for (const auto &item_states: states->second[item.first])
                {
                    if (item_states.first == "DESCRIPTION")
                    {
                        temp->set_description(item_states.second);
                    }
                    else if (item_states.first == "SURNAME")
                    {
                        temp->set_surname(item_states.second);
                    }
                    else if (item_states.first == "NAME")
                    {
                        temp->set_name(item_states.second);
                    }
                    else if (item_states.first == "PATRONYMIC")
                    {
                        temp->set_patronymic(item_states.second);
                    }
                    else if (item_states.first == "MAIL")
                    {
                        temp->set_mail(item_states.second);
                    }
                    else if (item_states.first == "PHONE")
                    {
                        temp->set_phone(item_states.second);
                    }
                    else if (item_states.first == "COMMENT")
                    {
                        temp->set_user_comment(item_states.second);
                    }
                    else if (item_states.first == "DATETIME")
                    {
                        temp->set_date_time_delivery(item_states.second);
                    }
                }
            }
        }
    }

    return temp;
}

void handler_get::get_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    size_t id_user = 0;
    size_t id_delivery = 0;
    type_value *get_target = nullptr;
    std::function<bool(const std::string&)> check_functions[8] = {
            check_description,
            check_surname,
            check_name,
            check_patronymic,
            check_mail,
            check_phone,
            check_user_comment,
            check_date_time_delivery
    };

    std::string names_field[8] = {
            "DESCRIPTION",
            "SURNAME",
            "NAME",
            "PATRONYMIC",
            "MAIL",
            "PHONE",
            "COMMENT",
            "DATETIME"
    };

    try
    {
        if (console)
        {
            int number_field = 0;
            std::string field_value;

            std::cout << "Choose field to be searched: " << std::endl;
            std::cout << "1. Id_user and id delivery" << std::endl;
            std::cout << "2. Description" << std::endl;
            std::cout << "3. Surname" << std::endl;
            std::cout << "4. Name" << std::endl;
            std::cout << "5. Patronymic" << std::endl;
            std::cout << "6. Mail" << std::endl;
            std::cout << "7. Phone" << std::endl;
            std::cout << "8. Comment" << std::endl;
            std::cout << "9. Datetime" << std::endl;
            std::cout << "Number field: ";
            std::cin >> number_field;

            if (number_field == 1)
            {
                read_id_data_id_delivery_from_console(id_user, id_delivery);

                if (collection->first->in(std::make_pair(id_user, id_delivery)))
                {
                    get_target = collection->first->get(std::make_pair(id_user, id_delivery));
                }
                else
                {
                    throw std::invalid_argument("ERROR: There is no such data_delivery");
                }
            }
            else if (number_field >= 2 && number_field <= 9)
            {
                std::cout << "PUT " + names_field[number_field - 2] + ": ";
                std::cin >> field_value;

                if (!check_functions[number_field - 2](field_value))
                {
                    throw std::invalid_argument("ERROR: Invalid field value");
                }

                if (collection->second[names_field[number_field - 2]]->in(field_value))
                {
                    get_target = collection->second[names_field[number_field - 2]]->get(field_value);
                }
                else
                {
                    throw std::invalid_argument("ERROR: There is no such data_delivery");
                }
            }
            else
            {
                throw std::invalid_argument("ERROR: Invalid number field");
            }
        }
        else
        {
            std::string name_field;
            std::string value_field;
            std::string line;

            if (stream.eof())
            {
                throw std::invalid_argument("ERROR: End of file");
            }

            std::getline(stream, name_field);
            name_field = handler::delete_spaces(name_field);

            if (name_field == "ID_USER_ID_DELIVERY")
            {
                try
                {
                    read_id_data_id_delivery(id_user, id_delivery, stream);
                }
                catch (const std::exception &ex)
                {
                    throw std::invalid_argument(ex.what());
                }

                get_target = collection->first->get(std::make_pair(id_user, id_delivery));
            }
            else if (name_field == "DESCRIPTION" ||
                     name_field == "SURNAME" ||
                     name_field == "NAME" ||
                     name_field == "PATRONYMIC" ||
                     name_field == "MAIL" ||
                     name_field == "PHONE" ||
                     name_field == "COMMENT" ||
                     name_field == "DATETIME")
            {
                if (stream.eof())
                {
                    throw std::invalid_argument("ERROR: End of file");
                }

                std::getline(stream, line);

                try
                {
                    read_arguments(line, name_field, ":", value_field);
                }
                catch (const std::exception &ex)
                {
                    throw std::invalid_argument(ex.what());
                }

                if (collection->second[name_field]->in(value_field))
                {
                    get_target = collection->second[name_field]->get(value_field);
                }
                else
                {
                    throw std::invalid_argument("ERROR: There is no such data_delivery");
                }
            }
            else
            {
                throw std::invalid_argument("ERROR: Invalid name field");
            }
        }
    }
    catch (const std::exception &ex)
    {
        throw std::invalid_argument(ex.what());
    }


    if (console)
    {
        delivery_data *temp = nullptr;
        std::vector<std::pair<std::string, std::string>> last_item;
        std::string datetime;
        long long _datetime = 0;
        int choice = 0;

        std::cout << "At what point in time do you want to receive data?" << std::endl;
        std::cout << "1. Current point in time" << std::endl;
        std::cout << "2. Set point in time" << std::endl;
        std::cout << "Put the number: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                temp = get_state("", get_target);
                std::cout << std::endl << "FOUND USER:" << std::endl << std::endl;
                output_delivery_data(temp);
                std::cout << std::endl;
                delete temp;

                break;
            case 2:
                std::cout << "Put the datetime in format %d/%m/%y-%h:%m:%s: ";
                std::cin >> datetime;

                _datetime = get_time(datetime);
                datetime = handler::delete_spaces(datetime);

                if (check_date_time_delivery(datetime))
                {
                    temp = get_state(datetime, get_target);
                    std::cout << std::endl << "FOUND USER AT THE POINT TIME " + datetime + ":" << std::endl << std::endl;
                    output_delivery_data(temp);
                    std::cout << std::endl;
                    delete temp;
                }
                else
                {
                    throw std::invalid_argument("ERROR: Invalid format datetime");
                }

                break;
            default:
                throw std::invalid_argument("ERROR: Invalid number to get item");
        }
    }
    else
    {
        std::string state;

        if (stream.eof())
        {
            throw std::invalid_argument("ERROR: File end");
        }

        std::getline(stream, state);
        state = handler::delete_spaces(state);

        if (state == "CURRENT")
        {
            delivery_data *temp = get_state("", get_target);
            std::cout << std::endl << "FOUND USER:" << std::endl << std::endl;
            output_delivery_data(temp);
            std::cout << std::endl;
            delete temp;
        }
        else if (check_date_time_delivery(state))
        {
            delivery_data *temp = get_state(state, get_target);
            std::cout << std::endl << "FOUND USER AT THE POINT TIME " + state + ":" << std::endl << std::endl;
            output_delivery_data(temp);
            std::cout << std::endl;
            delete temp;
        }
        else
        {
            throw std::invalid_argument("ERROR: Invalid state to get");
        }
    }
}

//end region handler_get


//begin region handler_get_range

void handler_get_range::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "GET_RANGE")
    {
        handler::get_logger()->log("GET_RANGE command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_get_range = new get_range_command;

        try
        {
            execute_get_range->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_get_range;
            throw std::invalid_argument(ex.what());
        }

        delete execute_get_range;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_get_range::get_range_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    size_t id_user_lower_bound = 0;
    size_t id_delivery_lower_bound = 0;
    size_t id_user_upper_bound = 0;
    size_t id_delivery_upper_bound = 0;
    std::vector<type_value*> res;
    std::function<bool(const std::string&)> check_functions[8] = {
            check_description,
            check_surname,
            check_name,
            check_patronymic,
            check_mail,
            check_phone,
            check_user_comment,
            check_date_time_delivery
    };

    std::string names_field[8] = {
            "DESCRIPTION",
            "SURNAME",
            "NAME",
            "PATRONYMIC",
            "MAIL",
            "PHONE",
            "COMMENT",
            "DATETIME"
    };

    try
    {
        if (console)
        {
            int number_field = 0;
            std::string lower_bound;
            std::string upper_bound;

            std::cout << "Choose field to be searched: " << std::endl;
            std::cout << "1. Id_user and id delivery" << std::endl;
            std::cout << "2. Description" << std::endl;
            std::cout << "3. Surname" << std::endl;
            std::cout << "4. Name" << std::endl;
            std::cout << "5. Patronymic" << std::endl;
            std::cout << "6. Mail" << std::endl;
            std::cout << "7. Phone" << std::endl;
            std::cout << "8. Comment" << std::endl;
            std::cout << "9. Datetime" << std::endl;
            std::cout << "Number field: ";
            std::cin >> number_field;

            try
            {
                if (number_field == 1)
                {
                    std::cout << "PUT LOWER BOUND: " << std::endl;
                    read_id_data_id_delivery_from_console(id_user_lower_bound, id_delivery_lower_bound);
                    std::cout << "PUT UPPER BOUND: " << std::endl;
                    read_id_data_id_delivery_from_console(id_user_upper_bound, id_delivery_upper_bound);

                    res = collection->first->get_range(std::make_pair(id_user_lower_bound, id_delivery_lower_bound),
                                                       std::make_pair(id_user_upper_bound, id_delivery_upper_bound));
                }
                else if (number_field >= 2 && number_field <= 9)
                {
                    std::cout << "PUT LOWER BOUND: " << std::endl;
                    std::cin >> lower_bound;
                    std::cout << "PUT UPPER BOUND: " << std::endl;
                    std::cin >> upper_bound;

                    if (!check_functions[number_field - 2](lower_bound) || !check_functions[number_field - 2](upper_bound))
                    {
                        throw std::invalid_argument("ERROR: Invalid description");
                    }

                    res = collection->second[names_field[number_field - 2]]->get_range(lower_bound, upper_bound);
                }
                else
                {
                    throw std::invalid_argument("ERROR: Invalid number_field");
                }
            }
            catch (const std::exception &ex)
            {
                throw std::invalid_argument(ex.what());
            }
        }
        else
        {
            std::string name_field;
            std::string lower_bound;
            std::string upper_bound;
            std::string line_lower_bound;
            std::string line_upper_bound;

            if (stream.eof())
            {
                throw std::invalid_argument("ERROR: End of file");
            }

            std::getline(stream, name_field);
            name_field = handler::delete_spaces(name_field);

            if (name_field == "ID_USER_ID_DELIVERY")
            {
                try
                {
                    read_id_data_id_delivery(id_user_lower_bound, id_delivery_lower_bound, stream);
                    read_id_data_id_delivery(id_user_upper_bound, id_delivery_upper_bound, stream);
                }
                catch (const std::exception &ex)
                {
                    throw std::invalid_argument(ex.what());
                }

                res = collection->first->get_range(std::make_pair(id_user_lower_bound, id_delivery_lower_bound),
                                                   std::make_pair(id_user_upper_bound, id_delivery_upper_bound));

            }
            else if (name_field == "DESCRIPTION" ||
                     name_field == "SURNAME" ||
                     name_field == "NAME" ||
                     name_field == "PATRONYMIC" ||
                     name_field == "MAIL" ||
                     name_field == "PHONE" ||
                     name_field == "COMMENT" ||
                     name_field == "DATETIME")
            {
                if (stream.eof())
                {
                    throw std::invalid_argument("ERROR: End of file");
                }

                std::getline(stream, line_lower_bound);

                if (stream.eof())
                {
                    throw std::invalid_argument("ERROR: End of file");
                }

                std::getline(stream, line_upper_bound);

                try
                {
                    read_arguments(line_lower_bound, name_field, ":", lower_bound);
                    read_arguments(line_upper_bound, name_field, ":", upper_bound);
                }
                catch (const std::exception &ex)
                {
                    throw std::invalid_argument(ex.what());
                }

                res = collection->second[name_field]->get_range(lower_bound, upper_bound);
            }
            else
            {
                throw std::invalid_argument("ERROR: Invalid name field");
            }
        }
    }
    catch (const std::exception &ex)
    {
        throw std::invalid_argument(ex.what());
    }

    delivery_data *temp = nullptr;

    if (res.empty())
    {
        std::cout << "NO MATCHES" << std::endl;
    }
    else
    {
        if (console)
        {
            std::vector<std::pair<std::string, std::string>> last_item;
            std::string datetime;
            long long _datetime = 0;
            int choice = 0;

            std::cout << "At what point in time do you want to receive data?" << std::endl;
            std::cout << "1. Current point in time" << std::endl;
            std::cout << "2. Set point in time" << std::endl;
            std::cout << "Put the number: ";
            std::cin >> choice;

            switch (choice)
            {
                case 1:
                    for (auto &item: res)
                    {
                        temp = get_state("", item);
                        output_delivery_data(temp);
                        std::cout << std::endl;
                        delete temp;
                    }

                    break;
                case 2:
                    std::cout << "Put the datetime in format %d/%m/%y-%h:%m:%s: ";
                    std::cin >> datetime;

                    _datetime = get_time(datetime);
                    datetime = handler::delete_spaces(datetime);

                    if (check_date_time_delivery(datetime))
                    {
                        for (auto &item: res)
                        {
                            temp = get_state(datetime, item);
                            output_delivery_data(temp);
                            std::cout << std::endl;
                            delete temp;
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("ERROR: Invalid format datetime");
                    }

                    break;
                default:
                    throw std::invalid_argument("ERROR: Invalid number to get item");
            }
        }
        else
        {
            std::string state;

            if (stream.eof())
            {
                throw std::invalid_argument("ERROR: File end");
            }

            std::getline(stream, state);
            state = handler::delete_spaces(state);

            if (state == "CURRENT")
            {
                for (auto &item: res)
                {
                    temp = get_state("", item);
                    output_delivery_data(temp);
                    std::cout << std::endl;
                    delete temp;
                }
            }
            else if (check_date_time_delivery(state))
            {
                for (auto &item: res)
                {
                    temp = get_state(state, item);
                    output_delivery_data(temp);
                    std::cout << std::endl;
                    delete temp;
                }
            }
            else
            {
                throw std::invalid_argument("ERROR: Invalid state to get");
            }
        }

        std::cout << std::endl << "FOUND USERS:" << std::endl << std::endl;
    }
}

//end region handler_get_range


//begin region handler_update

void handler_update::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "UPDATE")
    {
        handler::get_logger()->log("UPDATE command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*> scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_update = new update_command;

        try
        {
            execute_update->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_update;
            throw std::invalid_argument(ex.what());
        }

        delete execute_update;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_update::update_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    size_t id_user = 0;
    size_t id_delivery = 0;
    delivery_data to_insert;

    try
    {
        if (console)
        {
            read_id_data_id_delivery_from_console(id_user, id_delivery);
        }
        else
        {
            read_id_data_id_delivery(id_user, id_delivery, stream);
        }
    }
    catch (const std::exception &ex)
    {
        throw std::invalid_argument(ex.what());
    }

    if (collection->first->in(std::make_pair(id_user, id_delivery)))
    {
        type_value *update_item = collection->first->get(std::make_pair(id_user, id_delivery));
        long long current_time = get_time(get_current_datetime());

        if (console)
        {
            int number_field = -1;

            while (number_field != 0)
            {
                std::string new_value;

                std::cout << "Choose number to update some field or cancel" << std::endl;
                std::cout << "0. cancel" << std::endl;
                std::cout << "1. description" << std::endl;
                std::cout << "2. surname" << std::endl;
                std::cout << "3. name" << std::endl;
                std::cout << "4. patronymic" << std::endl;
                std::cout << "5. mail" << std::endl;
                std::cout << "6. phone" << std::endl;
                std::cout << "7. comment" << std::endl;
                std::cout << "8. datetime" << std::endl;
                std::cout << "Put the number: ";
                std::cin >> number_field;

                switch (number_field)
                {
                    case 0:
                        break;
                    case 1:
                        std::cout << "description: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("DESCRIPTION", new_value);
                        break;
                    case 2:
                        std::cout << "surname: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("SURNAME", new_value);
                        break;
                    case 3:
                        std::cout << "name: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("NAME", new_value);
                        break;
                    case 4:
                        std::cout << "patronymic: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("PATRONYMIC", new_value);
                        break;
                    case 5:
                        std::cout << "mail: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("MAIL", new_value);
                        break;
                    case 6:
                        std::cout << "phone: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("PHONE", new_value);
                        break;
                    case 7:
                        std::cout << "comment: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("COMMENT", new_value);
                        break;
                    case 8:
                        std::cout << "datetime: ";
                        std::cin >> new_value;
                        update_item->second[current_time].emplace_back("DATETIME", new_value);
                        break;
                    default:
                        throw std::invalid_argument("ERROR: Invalid number to update");
                }
            }
        }
        else
        {
            std::string line;

            while (line != "STOP_UPDATE")
            {
                if (stream.eof())
                {
                    throw std::invalid_argument("ERROR: Error update");
                }

                std::getline(stream, line);

                if (handler::delete_spaces(line) != "STOP_UPDATE")
                {
                    if (line.find(':') == std::string::npos)
                    {
                        throw std::invalid_argument("ERROR: Error field to update");
                    }

                    std::string name_argument = handler::delete_spaces(line.substr(0, line.find(':')));
                    std::string value_argument = handler::delete_spaces(line.substr(line.find(':') + 1));

                    if (name_argument == "DESCRIPTION")
                    {
                        update_item->second[current_time].emplace_back("description", value_argument);
                    }
                    else if (name_argument == "SURNAME")
                    {
                        update_item->second[current_time].emplace_back("surname", value_argument);
                    }
                    else if (name_argument == "NAME")
                    {
                        update_item->second[current_time].emplace_back("name", value_argument);
                    }
                    else if (name_argument == "PATRONYMIC")
                    {
                        update_item->second[current_time].emplace_back("patronymic", value_argument);
                    }
                    else if (name_argument == "MAIL")
                    {
                        update_item->second[current_time].emplace_back("mail", value_argument);
                    }
                    else if (name_argument == "PHONE")
                    {
                        update_item->second[current_time].emplace_back("phone", value_argument);
                    }
                    else if (name_argument == "COMMENT")
                    {
                        update_item->second[current_time].emplace_back("comment", value_argument);
                    }
                    else if (name_argument == "DATETIME")
                    {
                        update_item->second[current_time].emplace_back("datetime", value_argument);
                    }
                    else
                    {
                        throw std::invalid_argument("ERROR: Error field to update");
                    }
                }
            }
        }
    }
    else
    {
        throw std::invalid_argument("ERROR: There is no such delivery_data");
    }
}

//end region handler_update


//begin region handler_remove

void handler_remove::handle_request(
        const std::vector<std::string> &params,
        std::istream &stream,
        bool console)
{
    if (!params.empty() && params[0] == "REMOVE")
    {
        handler::get_logger()->log("REMOVE command called", logger::severity::INFORMATION);

        type_data_base *data_base = handler::get_instance();

        if (params.size() != 4)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: Invalid arguments to add collection");
        }

        if (!data_base->in(params[1]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such pool");
        }

        std::pair<type_pool*, memory*> pool = data_base->get(params[1]);

        if (!pool.first->in(params[2]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such data_scheme");
        }

        std::pair<type_scheme*, memory*>scheme = pool.first->get(params[2]);

        if (!scheme.first->in(params[3]))
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            throw std::invalid_argument("ERROR: There is no such collection");
        }

        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection = scheme.first->get(params[3]);

        command *execute_remove = new remove_command;

        try
        {
            execute_remove->execute(params, &pool, &scheme, collection, stream, console);
        }
        catch (const std::exception &ex)
        {
            handler::get_logger()->log("Exception caught", logger::severity::INFORMATION);
            delete execute_remove;
            throw std::invalid_argument(ex.what());
        }

        delete execute_remove;

        handler::get_logger()->log("Successful command execution", logger::severity::INFORMATION);
    }
    else
    {
        if (_next_handler != nullptr)
        {
            _next_handler->handle_request(params, stream, console);
        }
    }
}

void handler_remove::remove_command::execute(
        const std::vector<std::string> &params,
        std::pair<type_pool*, memory*> *pool,
        std::pair<type_scheme*, memory*> *scheme,
        std::pair<type_collection*, std::map<std::string, type_order_collection*>> *collection,
        std::istream &stream,
        bool console)
{
    size_t id_user = 0;
    size_t id_delivery = 0;

    try
    {
        if (console)
        {
            read_id_data_id_delivery_from_console(id_user, id_delivery);
        }
        else
        {
            read_id_data_id_delivery(id_user, id_delivery, stream);
        }
    }
    catch (const std::exception &ex)
    {
        throw std::invalid_argument(ex.what());
    }

    if (collection->first->in(std::make_pair(id_user, id_delivery)))
    {
        type_value *deleted_value = collection->first->get(std::make_pair(id_user, id_delivery));

        collection->second["DESCRIPTION"]->remove(deleted_value->first->get_description()->get_value());
        collection->second["SURNAME"]->remove(deleted_value->first->get_surname()->get_value());
        collection->second["NAME"]->remove(deleted_value->first->get_name()->get_value());
        collection->second["PATRONYMIC"]->remove(deleted_value->first->get_patronymic()->get_value());
        collection->second["MAIL"]->remove(deleted_value->first->get_mail()->get_value());
        collection->second["PHONE"]->remove(deleted_value->first->get_phone()->get_value());
        collection->second["COMMENT"]->remove(deleted_value->first->get_user_comment()->get_value());
        collection->second["DATETIME"]->remove(deleted_value->first->get_date_time_delivery()->get_value());
        collection->first->remove(std::make_pair(id_user, id_delivery));
    }
    else
    {
        throw std::invalid_argument("ERROR: There is no such data_delivery");
    }
}

//end region handler_remove


//begin region input/output

void read_id_data_id_delivery(
        size_t &id_user,
        size_t &id_delivery,
        std::istream &stream)
{
    std::string name_arguments[2] = {
            "USER_ID",
            "DELIVERY_ID"
    };

    int num_read_fields = 0;
    std::string fields[2];
    std::string line;

    for (int i = 0; i < 2 && !stream.eof(); i++)
    {
        std::getline(stream, line);

        try
        {
            read_arguments(line, name_arguments[i], ":", fields[num_read_fields++]);
        }
        catch (const std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }
    }

    if (num_read_fields != 2)
    {
        throw std::out_of_range("ERROR: Not enough data");
    }

    if (!check_id(fields[0]))
    {
        throw std::invalid_argument("ERROR: Invalid ID of USER");
    }

    if (!check_id(fields[1]))
    {
        throw std::invalid_argument("ERROR: Invalid ID of DELIVERY");
    }

    id_user = std::stoi(fields[0]);
    id_delivery = std::stoi(fields[1]);
}

void read_data_delivery(
        delivery_data *&target,
        std::istream &stream)
{
    target = new delivery_data;

    std::string name_arguments[LINES_TO_READ_DELIVERY] = {
            "DESCRIPTION",
            "SURNAME",
            "NAME",
            "PATRONYMIC",
            "MAIL",
            "PHONE",
            "COMMENT",
            "DATETIME"
    };

    int num_read_fields = 0;
    std::string fields[LINES_TO_READ_DELIVERY];
    std::string line;

    for (int i = 0; i < LINES_TO_READ_DELIVERY && !stream.eof(); i++)
    {
        std::getline(stream, line);

        try
        {
            read_arguments(line, name_arguments[i], ":", fields[num_read_fields++]);
        }
        catch (const std::exception &ex)
        {
            throw std::invalid_argument(ex.what());
        }
    }

    if (num_read_fields != LINES_TO_READ_DELIVERY)
    {
        throw std::out_of_range("ERROR: Not enough data");
    }

    if (!check_description(fields[0]))
    {
        throw std::invalid_argument("ERROR: Invalid description");
    }

    target->set_description(fields[0]);

    if (!check_surname(fields[1]))
    {
        throw std::invalid_argument("ERROR: Invalid surname");
    }

    target->set_surname(fields[1]);

    if (!check_name(fields[2]))
    {
        throw std::invalid_argument("ERROR: Invalid name");
    }

    target->set_name(fields[2]);

    if (!check_patronymic(fields[3]))
    {
        throw std::invalid_argument("ERROR: Invalid patronymic");
    }

    target->set_patronymic(fields[3]);

    if (!check_mail(fields[4]))
    {
        throw std::invalid_argument("ERROR: Invalid mail");
    }

    target->set_mail(fields[4]);

    if (!check_phone(fields[5]))
    {
        throw std::invalid_argument("ERROR: Invalid phone");
    }

    target->set_phone(fields[5]);

    if (!check_user_comment(fields[6]))
    {
        throw std::invalid_argument("ERROR: Invalid comment");
    }

    target->set_user_comment(fields[6]);

    if (!check_date_time_delivery(fields[7]))
    {
        throw std::invalid_argument("ERROR: Invalid date/time");
    }

    target->set_date_time_delivery(fields[7]);
}

void read_id_data_id_delivery_from_console(
        size_t &id_user,
        size_t &id_delivery)
{
    std::string id_user_string;
    std::string id_delivery_string;

    std::cout << "ID_USER: ";
    std::cin >> id_user_string;

    if (!check_id(id_user_string))
    {
        throw std::invalid_argument("ERROR: Invalid ID_USER");
    }

    std::cout << "ID_DELIVERY: ";
    std::cin >> id_delivery_string;

    if (!check_id(id_delivery_string))
    {
        throw std::invalid_argument("ERROR: Invalid ID_DELIVERY");
    }

    id_user = std::stoi(id_user_string);
    id_delivery = std::stoi(id_delivery_string);
}

void read_data_delivery_from_console(
        delivery_data *&target)
{
    std::string data;
    target = new delivery_data;

    std::cout << "DESCRIPTION: ";
    std::cin >> data;

    if (!check_description(data))
    {
        throw std::invalid_argument("ERROR: Invalid description");
    }

    target->set_description(data);


    std::cout << "SURNAME: ";
    std::cin >> data;

    if (!check_surname(data))
    {
        throw std::invalid_argument("ERROR: Invalid surname");
    }

    target->set_surname(data);


    std::cout << "NAME: ";
    std::cin >> data;

    if (!check_name(data))
    {
        throw std::invalid_argument("ERROR: Invalid name");
    }

    target->set_name(data);


    std::cout << "PATRONYMIC: ";
    std::cin >> data;

    if (!check_patronymic(data))
    {
        throw std::invalid_argument("ERROR: Invalid patronymic");
    }

    target->set_patronymic(data);


    std::cout << "MAIL: ";
    std::cin >> data;

    if (!check_mail(data))
    {
        throw std::invalid_argument("ERROR: Invalid mail");
    }

    target->set_mail(data);


    std::cout << "PHONE: ";
    std::cin >> data;

    if (!check_phone(data))
    {
        throw std::invalid_argument("ERROR: Invalid phone");
    }

    target->set_phone(data);


    std::cout << "COMMENT: ";
    std::cin >> data;

    if (!check_user_comment(data))
    {
        throw std::invalid_argument("ERROR: Invalid comment");
    }

    target->set_user_comment(data);


    std::cout << "DATE/TIME: ";
    std::cin >> data;

    if (!check_date_time_delivery(data))
    {
        throw std::invalid_argument("ERROR: Invalid date/time");
    }

    target->set_date_time_delivery(data);
}

void output_delivery_data(
        delivery_data *&target)
{
    std::cout << "DESCRIPTION: " << target->get_description()->get_value() << std::endl;
    std::cout << "SURNAME: " << target->get_surname()->get_value() << std::endl;
    std::cout << "NAME: " << target->get_name()->get_value() << std::endl;
    std::cout << "PATRONYMIC: " << target->get_patronymic()->get_value() << std::endl;
    std::cout << "MAIL: " << target->get_mail()->get_value() << std::endl;
    std::cout << "PHONE: " << target->get_phone()->get_value() << std::endl;
    std::cout << "USER_COMMENT: " << target->get_user_comment()->get_value() << std::endl;
    std::cout << "DATE_TIME_DELIVERY: " << target->get_date_time_delivery()->get_value() << std::endl;
}

void read_arguments(
        const std::string &str,
        const std::string &name_argument,
        const std::string &sep,
        std::string &value_argument)
{
    if (str.find(sep) == std::string::npos)
    {
        throw std::invalid_argument("ERROR: Invalid line of arguments");
    }

    std::string _name_argument = handler::delete_spaces(str.substr(0, str.find(sep)));

    if (_name_argument != name_argument)
    {
        throw std::invalid_argument("ERROR: Invalid name argument");
    }

    value_argument = handler::delete_spaces(str.substr(str.find(sep) + 1));
}

std::string get_current_datetime()
{
    std::time_t seconds = std::time(nullptr);
    std::tm* now = std::localtime(&seconds);

    char buffer[BUFSIZ];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %X", now);

    return buffer;
}

//05/09/2023 23:17:49

bool is_leap(
        int year)
{
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}

long long get_time(
        const std::string &time)
{
    int day_in_months[12] = {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };

    long long full_seconds = 0;
    long long full_days = 0;

    int day = std::stoi(time.substr(0, 2));
    int month = std::stoi(time.substr(3, 2));
    int year = std::stoi(time.substr(6, 4));
    int hours = std::stoi(time.substr(11, 2));
    int minutes = std::stoi(time.substr(14, 2));
    int seconds = std::stoi(time.substr(17, 2));

    full_days += day;
    full_days += (month == 2) ? (is_leap(year) ? day_in_months[month - 1] + 1 : day_in_months[month - 1]) : (day_in_months[month - 1]);

    for (int i = 0; i <= year; i++)
    {
        if (is_leap(i))
        {
            full_days += 366;
        }
        else
        {
            full_days += 365;
        }
    }

    full_seconds += seconds;
    full_seconds += minutes * 60;
    full_seconds += hours * 60 * 60;
    full_seconds += full_days * 24 * 60 * 60;

    return full_seconds;
}

//end region input/output