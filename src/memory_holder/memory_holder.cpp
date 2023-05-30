#include "memory_holder.h"

void memory_holder::deallocate_with_guard(
        void *object) const
{
    memory *alc = get_memory();

    alc == nullptr ? ::operator delete(object) : alc->deallocate(object);
}

void *memory_holder::allocate_with_guard(
        size_t size_allocate) const
{
    memory *alc = get_memory();

    return alc == nullptr ? ::operator new(size_allocate) : alc->allocate(size_allocate);
}