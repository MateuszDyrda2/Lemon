#pragma once

#include <lemon/core/basic_types.h>

#include <moodycamel/concurrentqueue.h>

#include <vector>

namespace lemon {
template<class T>
class object_pool
{
  public:
    ref<T> create();
    std::vector<ref<T>> create(size_type count);

  private:
    ref<moodycamel::ConcurrentQueue<ref<T>>> pool;
};
template<class T>
ref<T> object_pool<T>::create()
{
    ref<T> ret{};
    if(!pool->try_dequeue(ret))
    {
        ret = create_ref<T>();
    }
    return std::make_shared<T>(ret.get(), [=](T*) { pool->enqueue(ret); });
}
template<class T>
std::vector<ref<T>> object_pool<T>::create(size_type count)
{
    std::vector<ref<T>> ret{ count };
    auto rec = pool->try_dequeue_bulk(ret.begin(), count);
    for(auto iter = std::next(ret.begin(), rec); iter != ret.end(); ++iter)
    {
        *iter = create_ref<T>();
    }
    for(auto& r : ret)
    {
        r = std::make_shared<T>(r.get(), [=](T*) { pool->enqueue(r); });
    }
    return ret;
}
} // namespace lemon
