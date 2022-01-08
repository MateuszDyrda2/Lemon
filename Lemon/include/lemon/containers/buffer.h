#pragma once

#include <lemon/core/basic_types.h>

#include <iterator>

namespace lemon {
template<class T>
class buffer
{
  public:
    using self_type              = buffer<T>;
    using value_type             = T;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using difference_type        = std::ptrdiff_t;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  public:
    buffer();
    buffer(size_type size);
    buffer(const self_type&) = delete;
    buffer& operator=(const self_type&) = delete;
    buffer(self_type&& other) noexcept;
    buffer& operator=(self_type&& other) noexcept;
    ~buffer();
    void swap(buffer& other) noexcept;
    explicit operator bool() const noexcept;
    pointer get() const noexcept;
    size_type size() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

  private:
    pointer _buffer;
    size_type _size;
};
template<class T>
buffer<T>::buffer():
    _buffer{ nullptr }, _size{}
{ }
template<class T>
buffer<T>::buffer(size_type size):
    _buffer{ new T[size] }, _size(size)
{ }
template<class T>
buffer<T>::buffer(self_type&& other) noexcept:
    _buffer(other._buffer), _size(other._size)
{
    other._buffer = nullptr;
}
template<class T>
buffer<T>& buffer<T>::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        swap(other);
    }
    return *this;
}
template<class T>
buffer<T>::~buffer()
{
    if(_buffer) delete[] _buffer;
}
template<class T>
void buffer<T>::swap(buffer& other) noexcept
{
    std::swap(_buffer, other._buffer);
    std::swap(_size, other_size);
}
template<class T>
buffer<T>::operator bool() const noexcept
{
    return _buffer != nullptr;
}
template<class T>
typename buffer<T>::pointer
buffer<T>::get() const noexcept
{
    return _buffer;
}
template<class T>
size_type buffer<T>::size() const noexcept
{
    return _size;
}
template<class T>
inline typename buffer<T>::iterator
buffer<T>::begin() noexcept
{
    return _buffer;
}
template<class T>
inline typename buffer<T>::const_iterator
buffer<T>::begin() const noexcept
{
    return _buffer;
}
template<class T>
inline typename buffer<T>::const_iterator
buffer<T>::cbegin() const noexcept
{
    return _buffer;
}
template<class T>
inline typename buffer<T>::iterator
buffer<T>::end() noexcept
{
    return _buffer + _size;
}
template<class T>
inline typename buffer<T>::const_iterator
buffer<T>::end() const noexcept
{
    return _buffer + _size;
}
template<class T>
inline typename buffer<T>::const_iterator
buffer<T>::cend() const noexcept
{
    return _buffer + _size;
}
template<class T>
inline typename buffer<T>::reverse_iterator
buffer<T>::rbegin() noexcept
{
    return reverse_iterator(_buffer + _size);
}
template<class T>
inline typename buffer<T>::const_reverse_iterator
buffer<T>::rbegin() const noexcept
{
    return const_reverse_iterator(_buffer + _size);
}
template<class T>
inline typename buffer<T>::const_reverse_iterator
buffer<T>::crbegin() const noexcept
{
    return const_reverse_iterator(_buffer + _size);
}
template<class T>
inline typename buffer<T>::reverse_iterator
buffer<T>::rend() noexcept
{
    return reverse_iterator(_buffer);
}
template<class T>
inline typename buffer<T>::const_reverse_iterator
buffer<T>::rend() const noexcept
{
    return const_reverse_iterator(_buffer);
}
template<class T>
inline typename buffer<T>::const_reverse_iterator
buffer<T>::crend() const noexcept
{
    return const_reverse_iterator(_buffer);
}
} // namespace lemon
