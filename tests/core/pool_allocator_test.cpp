#include <gtest/gtest.h>

#include <core/pool_allocator.h>

using namespace lemon;

TEST(PoolAllocatorTest, CreatePoolAllocator)
{
    pool_allocator<int> pa;
}

TEST(PoolAllocatorTest, Get)
{
    pool_allocator<int> pa;
    pa.get();
}

TEST(PoolAllocatorTest, GetMultiple)
{
    pool_allocator<int, 2> pa;
    for (int i = 0; i < 1000; ++i)
    {
        pa.get();
    }
}

TEST(PoolAllocatorTest, Free)
{
    pool_allocator<int> pa;
    auto value = pa.get();
    pa.free(value);
}

TEST(PoolAllocatorTest, FreeMultiple)
{
    pool_allocator<int, 2> pa;
    int* arr[1000];
    for (int i = 0; i < 1000; ++i)
    {
        arr[i] = pa.get();
    }

    for (int i = 0; i < 1000; ++i)
    {
        pa.free(arr[i]);
    }
}
