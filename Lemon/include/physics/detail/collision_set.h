#pragma once

#include "world/system.h"
#include <algorithm>
#include <core/defines.h>
#include <core/lemon_types.h>

#include <set>
#include <type_traits>
#include <vector>

namespace lemon {
class LEMON_API collision_set
{
    using entity_type = std::underlying_type_t<entity_t>;
    using key_t       = u64;
    using is_correct  = std::integral_constant<bool, sizeof(key_t) == 2 * sizeof(entity_type)>;

  public:
    collision_set();
    ~collision_set() = default;

    bool has(entity_t a, entity_t b);
    bool push(entity_t a, entity_t b);
    void clear();
    void swap(collision_set& other) noexcept;
    template<class F>
    void diff(collision_set& other, F&& callable);

  private:
    std::set<key_t> container;

  private:
    key_t combine(entity_type a, entity_type b);
    std::pair<entity_t, entity_t> decouple(key_t key);
};

template<class F>
void collision_set::diff(collision_set& other, F&& callable)
{
    auto iter1 = container.begin();
    auto iter2 = other.container.begin();
    while(iter1 != container.end())
    {
        if(iter2 == other.container.end())
        {
            for(; iter2 != other.container.end(); ++iter2)
            {
                auto&& [f, s] = decouple(*iter2);
                callable(f, s);
            }
            break;
        }
        if(*iter1 < *iter2)
        {
            auto&& [f, s] = decouple(*iter1);
            callable(f, s);
        }
        else
        {
            if(!(*iter2 < *iter1))
            {
                ++iter1;
            }
            ++iter2;
        }
    }
}
}
