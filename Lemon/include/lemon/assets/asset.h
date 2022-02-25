#pragma once

#include "asset_storage.h"

#include <lemon/core/assert.h>
#include <lemon/core/defines.h>

namespace lemon {
/** @brief Class for accessing assets in the engine.
 * Contains a reference to the asset storage and key of a stored asset.
 */
template<class T>
class LEMON_PUBLIC asset
{
  public:
    using self_type       = asset<T>; ///< this type
    using pointer         = T*;       ///< pointer to stored resource
    using const_pointer   = const T*; ///< const pointer to stored resource
    using value_type      = T;        ///< type of the stored resource
    using reference       = T&;       ///< reference type of the stored resource
    using const_reference = const T&; ///< const reference type of the stored resource

  public:
    /** @brief Creates empty asset holder without
     * loading an actual asset
     */
    asset();
    /** @brief Creates an asset holder with for a specified asset id.
     * If the asset is already loaded, increments its reference count, otherwise
     * loads it to the engine.
     * @param name id of the asset to be loaded
     */
    asset(string_id name);
    /** @brief Copy constructs the asset, incremeting the reference count
     * of the represented asset
     * @param other asset to be copied
     */
    asset(const self_type& other);
    /** @brief Copies the other asset, incrementing its reference count
     * @param other asset to be copied
     * @return pointer to this
     */
    self_type& operator=(const self_type& other);
    /** @brief Move constructs the asset. The other assets state is invalid
     * after the move.
     * @param other asset to be moved
     */
    asset(self_type&& other) noexcept;
    /** @brief Moves the other asset to this. The other assets state is invalid
     * after the move.
     * @param other asset to be moved
     * @return pointer to this
     */
    self_type& operator=(self_type&& other) noexcept;
    ~asset();

    /** @return a const pointer to the asset stored in the object */
    const_pointer get() const noexcept;
    /** @return a pointer to the asset stored in the object */
    pointer get() noexcept;
    /** @brief Exchanges the assets the two asset holders represent
     * @param other other asset object to be swapped
     */
    void swap(self_type& other) noexcept;
    /** @brief Exchagnes the assets the two asset holders represent
     * @param lhs first asset to be swapped
     * @param rhs second asset to be swapped
     */
    friend void swap(self_type& lhs, self_type& rhs)
    {
        using std::swap;
        swap(lhs.res, rhs.res);
    }
    /** @brief Compares asset holders for whether they represent the same asset
     * @param other asset to check against
     * @return true if the assets represent the same
     */
    bool operator==(const self_type& other) const noexcept;
    inline string_id get_id() const { return res; }

  private:
    asset_storage& storage; ///< reference to the cached resources
    string_id res;          ///< id of the stored resource
};
template<class T>
asset<T>::asset():
    storage(*asset_storage::storage), res()
{ }
template<class T>
asset<T>::asset(string_id name):
    storage(*asset_storage::storage), res(name)
{
    storage.register_asset<T>(name);
}
template<class T>
asset<T>::asset(const self_type& other):
    storage(*asset_storage::storage), res(storage.clone_asset(other.res))
{ }
template<class T>
typename asset<T>::self_type&
asset<T>::operator=(const self_type& other)
{
    if(this != &other)
    {
        if(res) storage.release_asset(res);
        res = storage.clone_asset(other.res);
    }
    return *this;
}
template<class T>
asset<T>::asset(self_type&& other) noexcept:
    storage(*asset_storage::storage), res(other.res)
{
    other.res = nullptr;
}
template<class T>
typename asset<T>::self_type&
asset<T>::operator=(self_type&& other) noexcept
{
    if(this != &other)
    {
        if(res) storage.release_asset(res);
        res       = other.res;
        other.res = nullptr;
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
    return storage.get_asset<T>(res);
}
template<class T>
typename asset<T>::pointer
asset<T>::get() noexcept
{
    return storage.get_asset<T>(res);
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
