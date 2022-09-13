#pragma once

#include <core/assert.h>

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace lemon {
class type_erased_array
{
  public:
    type_erased_array(std::size_t elementSize):
        elementSize(elementSize) { }
    ~type_erased_array()
    {
        if(head) std::free(head);
    }
    inline void* push()
    {
        if(size >= capacity) realloc();
        return head + size++;
    }
    inline void* push(void* element)
    {
        if(size >= capacity) realloc();
        std::memcpy(pointer_from_index(size), element, elementSize);
        return head + size++;
    }

    inline void swap_and_discard(std::size_t target)
    {
        lemon_assert(target < capacity);
        std::memcpy(pointer_from_index(target),
                    pointer_from_index(size - 1),
                    elementSize);
        --size;
    }

    template<class T, class Args>
    inline T& push(Args&&... args)
    {
        if(size >= capacity) realloc();
        auto res =
            std::construct_at(reinterpret_cast<T*>(pointer_from_index(size++)),
                              std::forward<Args>(args)...);
        return *res;
    }

    template<class T>
    inline T& push(const T& val)
    {
        if(size >= capacity) realloc();
        auto res =
            std::construct_at(reinterpret_cast<T*>(pointer_from_index(size++)),
                              val);
        return res;
    }

    template<class T>
    inline T& push(T&& val)
    {
        if(size >= capacity) realloc();
        auto res =
            std::construct_at(reinterpret_cast<T*>(pointer_from_index(size++)),
                              std::move(val));
        return res;
    }

    inline void pop()
    {
        --size;
    }
    inline void* at(std::size_t index)
    {
        lemon_assert(index < size);
        return (pointer_from_index(index));
    }
    inline void* get()
    {
        return head;
    }

  private:
    unsigned char* head  = nullptr;
    std::size_t size     = 0ULL;
    std::size_t capacity = 0ULL;
    std::size_t elementSize;

  private:
    inline void realloc()
    {
        capacity     = 2 * capacity;
        auto newHead = (unsigned char*)std::malloc(capacity * elementSize);
        std::memcpy(newHead, head, size * elementSize);
        if(head) std::free(head);
        head = newHead;
    }
    inline void* pointer_from_index(std::size_t index)
    {
        return (head + (index * elementSize));
    }
};
}
