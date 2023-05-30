#ifndef ALLOCATOR_BUDDIES_H
#define ALLOCATOR_BUDDIES_H

#include "logger.h"
#include "logger_concrete.h"
#include "builder.h"
#include "builder_concrete.h"
#include "memory.h"
#include <iostream>

class allocator_buddies final : public memory
{
    
private:
    
    void *_allocated_memory;

private:
    
    void *get_first_free_block() const;

    short get_power_block(
            void *block_memory) const;

    void *get_prev_free_block(
            void *block_memory) const;

    void *get_next_free_block(
            void *block_memory) const;

    void *get_buddies(
            void *block_memory) const;

    bool is_free_block(
            void *block_memory) const;

    logger *get_logger() const;

    memory *get_memory() const;
    
public:
    
    allocator_buddies(allocator_buddies const&) = delete;

    void operator=(allocator_buddies const&) = delete;

    explicit allocator_buddies(
            size_t const &size,
            logger *logger_allocator = nullptr,
            memory *outer_allocator = nullptr);

    void *allocate(size_t const &size) const override;

    void deallocate(void *deallocated_block) const override;

    ~allocator_buddies() override;
    
};

#endif //ALLOCATOR_BUDDIES_H