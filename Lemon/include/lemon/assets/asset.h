#pragma once

#include "asset_storage.h"
#include <lemon/core/assert.h>

namespace lemon {

template<class T>
class asset
{
  public:
    using self_type       = asset<T>;
    using pointer         = T*;
    using const_pointer   = const T*;
    using value_type      = T;
    using reference       = T&;
    using const_reference = const T&;

  public:
    asset();
    asset(string_id name);
    asset(const self_type& other);
    self_type& operator=(const self_type& other);
    asset(self_type&& other) noexcept;
    self_type& operator=(self_type&& other) noexcept;
    ~asset();

    const_pointer get() const noexcept;
    pointer get() noexcept;

    void swap(self_type& other) noexcept;
    friend void swap(self_type& lhs, self_type& rhs)
    {
        using std::swap;
        swap(lhs.res, rhs.res);
    }

    bool operator==(const self_type& other) const noexcept;

  private:
    string_id res;
    static asset_storage storage;
};
template<class T>
asset<T>::asset():
    res{}
{ }
template<class T>
asset<T>::asset(string_id name):
    res{ name }
{
    storage.register_asset(name);
}
template<class T>
asset<T>::asset(const self_type& other):
    res{ other.res }
{
    storage.clone_asset(res);
}
template<class T>
typename asset<T>::self_type&
asset<T>::operator=(const self_type& other)
{
    if(this != &other)
    {
        if(res) storage.release_asset(res);
        res = other.res;
        storage.clone_asset(res);
    }
    return *this;
}
template<class T>
asset<T>::asset(self_type&& other) noexcept:
    res{ other.res }
{
    other.res = nullptr;
}
template<class T>
typename asset<T>::self_type&
asset<T>::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        std::swap(res, other.res);
    }
    return *this;
}
template<class T>
asset<T>::~asset()
{
    if(res) storage.release_asset(res);
}
template<class T>
typename asset<T>::const_pointer
asset<T>::get() const noexcept
{
    return storage.get_asset<T>(name);
}
template<class T>
typename asset<T>::pointer
asset<T>::get() noexcept
{
    return storage.get_asset<T>(name);
}
template<class T>
bool asset<T>::operator==(const self_type& other) const noexcept
{
    return res == other.res;
}
template<class T>
void asset<T>::swap(self_type& other) noexcept
{
    std::swap(res, other.res);
}
} // namespace lemon
