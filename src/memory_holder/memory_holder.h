#ifndef MEMORY_HOLDER_H
#define MEMORY_HOLDER_H

#include "memory.h"

class memory_holder
{

public:

    virtual ~memory_holder() = default;

    void deallocate_with_guard(
            void *object) const;

    void *allocate_with_guard(
            size_t size_allocate) const;

    virtual memory *get_memory() const = 0;

};

#endif //MEMORY_HOLDER_H