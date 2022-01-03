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
    resource(const resource<T>& other):
        res(other.res) { }
    template<class W>
    resource(const resource<W>& other):
        res(static_cast<T*>(other.res)) { }
    ~resource() { }
    resource<T>& operator=(const resource<T>&) = default;
    template<class W>
    resource<T>& operator=(const resource<W>& other)
    {
        if(this != &other)
        {
            res = static_cast<T*>(other.res);
        }
        return *this;
    }

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
