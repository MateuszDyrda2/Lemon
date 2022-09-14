#pragma once

#include "world_primitives.h"

#include <iterator>
#include <utility>

namespace lemon {
struct cquery
{
    void* components;
    componentdef componentDef;
};

template<class... Args>
struct query
{
    template<class T>
    using pointer = T*;
    std::tuple<pointer<T>...> components;
    std::size_t count;
};

void f()
{
    query<int, float> a;
    auto [in, fl] = a.components;
    for(std::size_t i = 0; i < a.count; ++i)
    {
        printf("%d %f", in[i], fl[i]);
    }
}
}
