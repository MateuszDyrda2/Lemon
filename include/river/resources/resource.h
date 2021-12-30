#pragma once

#include <river/core/assert.h>
#include <river/core/basic_types.h>
#include <river/game/object.h>

namespace river {
template<class T>
class resource
{
  public:
    resource():
        res{ nullptr } { }
    resource(T* res):
        res(res)
    { }
    ~resource();
    T& operator*()
    {
        RIVER_ASSERT(res);
        return *res;
    }
    const T& operator*() const
    {
        RIVER_ASSERT(res);
        return *res;
    }
    T* operator->()
    {
        RIVER_ASSERT(res);
        return res;
    }
    const T* operator->() const
    {
        RIVER_ASSERT(res);
        return res;
    }
    bool operator==(const resource<T>& other)
    {
        return res == other.res;
    }

  private:
    T* res;
};
} // namespace river
