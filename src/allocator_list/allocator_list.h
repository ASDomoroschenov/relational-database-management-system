#ifndef ALLOCATOR_LIST_H
#define ALLOCATOR_LIST_H

#include "logger.h"
#include "logger_concrete.h"
#include "builder.h"
#include "builder_concrete.h"
#include "memory.h"
#include <iostream>

class allocator_list final : public memory
{
private:

    void *_allocated_memory;
    memory::METHOD_SUITABLE _method;

public:

    allocator_list(
            allocator_list const&) = delete;

    void operator=(
            allocator_list const&) = delete;

private:

    size_t get_memory_size() const;

    logger *get_logger() const;

    memory *get_memory() const;

    void *get_next_free_block(
            void *memory_block) const;

    size_t get_block_size(
            void const *memory_block) const;

    void *get_first_free_block() const;

    void *get_first_suitable_block(
            size_t const &size,
            void **prev_block) const;

    void *get_best_suitable_block(
            size_t const &size,
            void **prev_block) const;

    void *get_worse_suitable_block(
            size_t const &size,
            void **prev_block) const;

    void compact_memory() const;

public:

    explicit allocator_list(
            size_t const &size,
            memory *outer_allocator_list = nullptr,
            logger *logger_allocator_list = nullptr,
            memory::METHOD_SUITABLE method_allocation = memory::METHOD_SUITABLE::FIRST_SUITABLE);

    void *allocate(
            size_t const &size) const override;

    void deallocate(
            void *deallocated_block) const override;

    ~allocator_list() override;
};

#endif // ALLOCATOR_LIST_H