#ifndef ALLOCATOR_DESCRIPTOR_H
#define ALLOCATOR_DESCRIPTOR_H

#include "logger.h"
#include "logger_concrete.h"
#include "builder.h"
#include "builder_concrete.h"
#include "memory.h"
#include <iostream>

class allocator_descriptor final : public memory
{

private:

    void *_allocated_memory;
    memory::METHOD_SUITABLE _method;

public:

    allocator_descriptor(allocator_descriptor const&) = delete;

    void operator=(allocator_descriptor const&) = delete;

private:

    logger *get_logger() const;

    memory *get_memory() const;

    void *get_next_occupied_block(void *memory_block) const;

    size_t get_memory_size() const;

    size_t get_block_size(void const *memory_block) const;

    void *get_first_occupied_block() const;

    void *get_first_suitable_block(
            size_t const &size,
            void **prev_block,
            void **next_block) const;

    void *get_best_suitable_block(
            size_t const &size,
            void **prev_block,
            void **next_block) const;

    void *get_worse_suitable_block(
            size_t const &size,
            void **prev_block,
            void **next_block) const;

public:

    explicit allocator_descriptor(
            size_t const &size,
            memory *outer_allocator = nullptr,
            logger *logger_allocator = nullptr,
            memory::METHOD_SUITABLE method_allocation = memory::METHOD_SUITABLE::FIRST_SUITABLE);

    void *allocate(
            size_t const &size) const override;

    void deallocate(void *deallocated_block) const override;

    ~allocator_descriptor() override;
};

#endif //ALLOCATOR_DESCRIPTOR_H