/** @file pool_allocator.h
 * @brief File with a pool allocator definition
 */
#pragma once

#include "defines.h"
#include "lemon_types.h"

#include <array>
#include <cstdint>
#include <list>
#include <type_traits>

namespace lemon {
/** Pool allocator
 * @tparam T object type
 * @tparam BlockSize size of a block
 * @tparam ChunkSize size of a chunk
 */
template<class T, std::size_t BlockSize = 32,
         std::size_t ChunkSize = (sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*))>
class pool_allocator
{
  private:
    struct alignas(T) chunk_t
    {
        chunk_t* next{ nullptr };
    };

  public:
    /** @brief Creates a pool allocator */
    pool_allocator();
    /** @brief Get a pointer to a free object
     * @return Free object pointer
     */
    T* get();
    /** @brief Free an element
     * @param element pointer to element to be freed
     */
    void free(T* element);
    /** @brief Clear the memory from the allocator */
    void clear();

  private:
    std::list<std::aligned_storage_t<BlockSize * ChunkSize, alignof(T)>> blocks;
    chunk_t* head{ nullptr };

    void add_block();
};

template<class T, std::size_t BlockSize, std::size_t ChunkSize>
pool_allocator<T, BlockSize, ChunkSize>::pool_allocator()
{
    add_block();
}

template<class T, std::size_t BlockSize, std::size_t ChunkSize>
inline T* pool_allocator<T, BlockSize, ChunkSize>::get()
{
    if (head == nullptr)
    {
        add_block();
    }
    auto res = head;
    head     = head->next;
    return reinterpret_cast<T*>(reinterpret_cast<void*>(res));
}

template<class T, std::size_t BlockSize, std::size_t ChunkSize>
inline void pool_allocator<T, BlockSize, ChunkSize>::free(T* element)
{
    auto toInsert  = reinterpret_cast<chunk_t*>(reinterpret_cast<void*>(element));
    toInsert->next = head;
    head           = toInsert;
}

template<class T, std::size_t BlockSize, std::size_t ChunkSize>
inline void pool_allocator<T, BlockSize, ChunkSize>::add_block()
{
    auto&& inserted = blocks.emplace_back();
    auto start      = reinterpret_cast<byte*>(&inserted);
    head            = reinterpret_cast<chunk_t*>(start);
    for (std::size_t i = 0; i < BlockSize - 1; ++i)
    {
        auto nextAddress                        = reinterpret_cast<std::uintptr_t>(start) + ChunkSize;
        reinterpret_cast<chunk_t*>(start)->next = reinterpret_cast<chunk_t*>(nextAddress);
        start                                   = reinterpret_cast<byte*>(nextAddress);
    }
    reinterpret_cast<chunk_t*>(start)->next = nullptr;
}

template<class T, std::size_t BlockSize, std::size_t ChunkSize>
void pool_allocator<T, BlockSize, ChunkSize>::clear()
{
    blocks.clear();
    head = nullptr;
    add_block();
}
}
