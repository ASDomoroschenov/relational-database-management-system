#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>

class memory
{

public:

    enum class METHOD_SUITABLE
    {
        FIRST_SUITABLE,
        BEST_SUITABLE,
        WORSE_SUITABLE
    };

    memory(
            memory const&) = delete;
    
    void operator=(
            memory const&) = delete;

    memory() = default;
    
    virtual ~memory() = default;

    virtual void *allocate(
            size_t const&) const = 0;

    void *operator+=(
            size_t const &size) const;

    virtual void deallocate(
            void*) const = 0;

    void operator-=(
            void*) const;

};

#endif // MEMORY_H