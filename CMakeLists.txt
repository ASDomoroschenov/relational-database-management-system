cmake_minimum_required(VERSION 3.24)
project(coursework)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-w)

include_directories(src/associative_container)
include_directories(src/binary_search_tree)
include_directories(src/memory)
include_directories(src/logger)
include_directories(src/logger_concrete)
include_directories(src/builder)
include_directories(src/builder_concrete)
include_directories(src/avl_tree)
include_directories(src/memory_holder)
include_directories(src/logger_holder)
include_directories(src/red_black_tree)
include_directories(src/allocator_list)
include_directories(src/allocator_descriptor)
include_directories(src/allocator_buddies)
include_directories(src/splay_tree)
include_directories(src/check_data)
include_directories(src/handle_request)
include_directories(src/handle_request_concrete)
include_directories(src/client)
include_directories(src/client_concrete)
include_directories(src/flyweight)
include_directories(src/delivery_data)
include_directories(src/types)
include_directories(src/logger_json_builder)
include_directories(src/logger_json_builder_concrete)

add_executable(
        coursework

        src/builder/builder.h
        src/builder/builder.cpp

        src/builder_concrete/builder_concrete.h
        src/builder_concrete/builder_concrete.cpp

        src/logger/logger.h
        src/logger/logger.cpp

        src/logger_concrete/logger_concrete.h
        src/logger_concrete/logger_concrete.cpp

        src/logger_holder/logger_holder.h
        src/logger_holder/logger_holder.cpp

        src/memory/memory.h
        src/memory/memory.cpp

        src/allocator_list/allocator_list.h
        src/allocator_list/allocator_list.cpp

        src/allocator_descriptor/allocator_descriptor.h
        src/allocator_descriptor/allocator_descriptor.cpp

        src/allocator_buddies/allocator_buddies.h
        src/allocator_buddies/allocator_buddies.cpp

        src/memory_holder/memory_holder.h
        src/memory_holder/memory_holder.cpp

        src/associative_container/associative_container.h
        src/associative_container/associative_container.cpp

        src/binary_search_tree/binary_search_tree.h
        src/binary_search_tree/binary_search_tree.cpp

        src/avl_tree/avl_tree.h
        src/avl_tree/avl_tree.cpp

        src/red_black_tree/red_black_tree.h
        src/red_black_tree/red_black_tree.cpp

        src/splay_tree/splay_tree.h
        src/splay_tree/splay_tree.cpp

        src/types/types.h

        src/check_data/check_data.h
        src/check_data/check_data.cpp

        src/handle_request/handle_request.h
        src/handle_request/handle_request.cpp

        src/handle_request_concrete/handle_request_concrete.h
        src/handle_request_concrete/handle_request_concrete.cpp

        src/client/client.h
        src/client/client.cpp

        src/client_concrete/client_concrete.h
        src/client_concrete/client_concrete.cpp

        src/flyweight/flyweight.h
        src/flyweight/flyweight.cpp

        src/delivery_data/delivery_data.h
        src/delivery_data/delivery_data.cpp

        src/logger_json_builder/logger_json_builder.h
        src/logger_json_builder/logger_json_builder.cpp

        src/logger_json_builder_concrete/logger_json_builder_concrete.cpp
        src/logger_json_builder_concrete/logger_json_builder_concrete.h

        src/main.cpp)
