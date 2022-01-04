#pragma once

#include <lemon/core/assert.h>
#include <lemon/core/basic_types.h>
#include <lemon/game/object.h>

namespace lemon {
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
        LEMON_ASSERT(res);
        return *res;
    }
    const T& operator*() const
    {
        LEMON_ASSERT(res);
        return *res;
    }
    T* operator->()
    {
        LEMON_ASSERT(res);
        return res;
    }
    const T* operator->() const
    {
        LEMON_ASSERT(res);
        return res;
    }
    bool operator==(const resource<T>& other)
    {
        return res == other.res;
    }

  private:
    T* res;
};
} // namespace lemon
