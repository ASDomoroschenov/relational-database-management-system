#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include "red_black_tree.h"
#include "avl_tree.h"
#include "splay_tree.h"
#include "delivery_data.h"
#include <map>
#include <memory>

class compare_str_keys
{

public:

    int operator()(
            const std::string &first_cmp,
            const std::string &second_cmp) const
    {
        return first_cmp.compare(second_cmp);
    }

};

class compare_data_keys
{

public:

    int operator()(
            const std::pair<size_t, size_t> &first_cmp,
            const std::pair<size_t, size_t> &second_cmp) const
    {
        if (first_cmp.first > second_cmp.first)
        {
            return 1;
        }

        if (first_cmp.first < second_cmp.first)
        {
            return -1;
        }

        if (first_cmp.first == second_cmp.first)
        {
            if (first_cmp.second > second_cmp.second)
            {
                return 1;
            }

            if (first_cmp.second < second_cmp.second)
            {
                return -1;
            }
        }

        return 0;
    }
};

enum class RELATION_ORDER
{
    PLUG,
    DESCRIPTION,
    SURNAME,
    NAME,
    PATRONYMIC,
    MAIL,
    PHONE,
    COMMENT,
    DATETIME
};

class order_compare
{

public:

    int operator()(
            const std::pair<delivery_data*, RELATION_ORDER*> &first_item,
            const std::pair<delivery_data*, RELATION_ORDER*> &second_item) const
    {
        return 1;
    }
};

using type_value = std::pair<delivery_data*, std::map<long long, std::vector<std::pair<std::string, std::string>>>>;
using type_states = std::map<long long, std::vector<std::pair<std::string, std::string>>>;
using type_order_collection = red_black_tree
        <
            std::string,
            type_value*,
            compare_str_keys
        >;


using type_collection = associative_container
        <
            std::pair<size_t, size_t>,
            type_value*
        >;

using type_scheme = associative_container
        <
            std::string,
            std::pair<type_collection*, std::map<std::string, type_order_collection*>>*
        >;

using type_pool = associative_container
        <
            std::string,
            std::pair<type_scheme*, memory*>
        >;

using type_data_base = associative_container
        <
            std::string,
            std::pair<type_pool*, memory*>
        >;




using type_collection_rb = red_black_tree
        <
                std::pair<size_t, size_t>,
                type_value*,
                compare_data_keys
        >;

using type_scheme_rb = red_black_tree
        <
                std::string,
                std::pair<type_collection_rb*, std::map<std::string, type_order_collection*>>*,
                compare_str_keys
        >;

using type_pool_rb = red_black_tree
        <
                std::string,
                std::pair<type_scheme_rb*, memory*>,
                compare_str_keys
        >;

using type_data_base_rb = red_black_tree
        <
                std::string,
                std::pair<type_pool_rb*, memory*>,
                compare_str_keys
        >;





using type_collection_avl = avl_tree
        <
                std::pair<size_t, size_t>,
                type_value*,
                compare_data_keys
        >;

using type_scheme_avl = avl_tree
        <
                std::string,
                std::pair<type_collection_avl*, std::map<std::string, type_order_collection*>>*,
                compare_str_keys
        >;

using type_pool_avl = avl_tree
        <
                std::string,
                std::pair<type_scheme_rb*, memory*>,
                compare_str_keys
        >;

using type_data_base_avl = avl_tree
        <
                std::string,
                std::pair<type_pool_rb*, memory*>,
                compare_str_keys
        >;




using type_collection_splay = splay_tree
        <
                std::pair<size_t, size_t>,
                type_value*,
                compare_data_keys
        >;

using type_scheme_splay = splay_tree
        <
                std::string,
                std::pair<type_collection_splay*, std::map<std::string, type_order_collection*>>*,
                compare_str_keys
        >;

using type_pool_splay = splay_tree
        <
                std::string,
                std::pair<type_scheme_rb*, memory*>,
                compare_str_keys
        >;

using type_data_base_splay = splay_tree
        <
                std::string,
                std::pair<type_pool_rb*, memory*>,
                compare_str_keys
        >;

#endif //TYPES_H