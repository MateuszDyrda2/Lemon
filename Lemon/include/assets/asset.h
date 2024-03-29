/** @file asset_storage.h
 * @brief Caching and managing game resources
 */
#pragma once

#include <core/defines.h>
#include <core/logger.h>

#include "asset_loader.h"
#include "resource.h"
#include "scripting/scripting_engine.h"

/** @TODO: Loading the data from files should take place on a seperate thread
 * @TODO: Add unit tests
 */
namespace lemon {
class asset_storage;
/** @brief Class for accessing assets in the engine.
 * Contains a reference to the asset storage and key of a stored asset.
 */
template<class T>
class asset
{
  public:
    /** @brief Creates empty asset holder without
     * loading an actual asset
     */
    asset();
    /** @brief Copy constructs the asset, incremeting the reference count
     * of the represented asset
     * @param other asset to be copied
     */
    asset(const asset<T>& other);
    /** @brief Copies the other asset, incrementing its reference count
     * @param other asset to be copied
     * @return pointer to this
     */
    asset<T>& operator=(const asset<T>& other);
    /** @brief Move constructs the asset. The other assets state is invalid
     * after the move.
     * @param other asset to be moved
     */
    asset(asset<T>&& other) noexcept;
    /** @brief Moves the other asset to this. The other assets state is invalid
     * after the move.
     * @param other asset to be moved
     * @return pointer to this
     */
    asset<T>& operator=(asset<T>&& other) noexcept;
    ~asset();
    /** @return a const pointer to the asset stored in the object */
    const T* get() const noexcept;
    /** @return a pointer to the asset stored in the object */
    T* get() noexcept;
    /** @brief Exchanges the assets the two asset holders represent
     * @param other other asset object to be swapped
     */
    void swap(asset<T>& other) noexcept;
    /** @brief Exchanges the assets the two asset holders represent
     * @param lhs first asset to be swapped
     * @param rhs second asset to be swapped
     */
    friend void swap(asset<T>& lhs, asset<T>& rhs)
    {
        std::swap(lhs.res, rhs.res);
    }
    /** @brief Compares asset holders for whether they represent the same asset
     * @param other asset to check against
     * @return true if the assets represent the same
     */
    bool operator==(const asset<T>& other) const noexcept;
    bool operator!=(const asset<T>& other) const noexcept;
    explicit operator bool() const noexcept { return bool(res); }
    inline hashstr get_id() const { return res; }

  private:
    friend class asset_storage;
    asset_storage* storage; ///< reference to the cached resources
    hashstr res;            ///< id of the stored resource

  private:
    asset(hashstr name, asset_storage* storage);
};
/** Class storing the actual assets, allowing for caching assets
 * that are used in many places and managing their lifetime.
 */
class LEMON_API asset_storage
{
  public:
    /** @brief Create asset storage */
    asset_storage(const std::string& path, scripting_engine& _scriptingEngine);
    ~asset_storage();

    /** @brief Create and return an asset
     * @param nameid id of the asset
     * @return asset
     */
    template<class T>
    [[nodiscard]] asset<T> get_asset(hashstr nameid);
    /** @brief Remove not used resources */
    void update();

  private:
    template<class T>
    T* get_asset_ptr(hashstr name) const;

    void release_asset(hashstr name);
    hashstr clone_asset(hashstr name);

  private:
    std::unordered_map<hashstr, std::unique_ptr<resource>>
        cachedAssets;                     ///< map of the resources
    std::unique_ptr<asset_loader> loader; ///< asset loader
    std::vector<std::unique_ptr<resource>> toDelete;
    template<class T>
    friend class asset;

  private:
    template<class T>
    T* get_mock_asset() const;
};

template<class T>
asset<T> asset_storage::get_asset(hashstr nameid)
{
    lemon_assert(nameid);
    auto res = cachedAssets.find(nameid);
    if (res == cachedAssets.end())
    {
        lemon_assert(loader->resource_exists(nameid));

        cachedAssets.insert(std::make_pair(nameid, loader->load_resource<T>(nameid)));
        return asset<T>(nameid, this);
    }

    res->second->increment();
    return asset<T>(nameid, this);
}

template<class T>
T* asset_storage::get_mock_asset() const
{
    logger::fatal("Not implemented yet");
    return static_cast<T*>(cachedAssets.at(T::get_mocked()).get());
}

template<class T>
T* asset_storage::get_asset_ptr(hashstr nameid) const
{
    if (!nameid) return nullptr;

    auto res = cachedAssets.find(nameid);
    if (res == cachedAssets.end()) return get_mock_asset<T>();

    return static_cast<T*>(res->second.get());
}

template<class T>
asset<T>::asset():
    storage{}, res()
{ }

template<class T>
asset<T>::asset(hashstr name, asset_storage* storage):
    storage(storage), res(name)
{ }

template<class T>
asset<T>::asset(const asset<T>& other):
    storage(other.storage), res(storage ? storage->clone_asset(other.res) : other.res)
{ }

template<class T>
asset<T>&
asset<T>::operator=(const asset<T>& other)
{
    if (this != &other)
    {
        if (res) storage->release_asset(res);
        if (storage)
            res = storage->clone_asset(other.res);
    }
    return *this;
}

template<class T>
asset<T>::asset(asset<T>&& other) noexcept:
    storage(other.storage), res(other.res)
{
    other.res = hashstr();
}

template<class T>
asset<T>&
asset<T>::operator=(asset<T>&& other) noexcept
{
    if (this != &other)
    {
        std::swap(res, other.res);
        std::swap(storage, other.storage);
    }
    return *this;
}

template<class T>
asset<T>::~asset()
{
    if (!storage || !res) return;

    storage->release_asset(res);
}

template<class T>
const T*
asset<T>::get() const noexcept
{
    lemon_assert(storage && res);
    return storage->get_asset_ptr<T>(res);
}

template<class T>
T* asset<T>::get() noexcept
{
    lemon_assert(storage && res);
    return storage->get_asset_ptr<T>(res);
}

template<class T>
bool asset<T>::operator==(const asset<T>& other) const noexcept
{
    return res == other.res;
}

template<class T>
bool asset<T>::operator!=(const asset<T>& other) const noexcept
{
    return !(*this == other);
}

template<class T>
void asset<T>::swap(asset<T>& other) noexcept
{
    std::swap(res, other.res);
}
} // namespace lemon
