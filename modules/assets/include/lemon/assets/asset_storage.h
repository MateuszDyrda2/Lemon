#pragma once

#include "asset_loader.h"
#include "resource.h"

#include <lemon/core/defines.h>

/** @TODO: Resources should be destroyed in a seperate update method in the render thread
 * @TODO: Loading the data from files should take place on a seperate thread
 */
namespace lemon {
/** Class storing the actual assets, allowing for caching assets
 * that are used in many places and managing their lifetime.
 */
class LEMON_PUBLIC asset_storage
{
  private:
  public:
    using self_type      = asset_storage;
    using container_type = std::unordered_map<string_id, owned<resource>>;

  public:
    /** @brief Creates the asset storage and asset loader
     * from the specified path
     * @param dataPath path to the file with the asset paths
     */
    asset_storage();
    ~asset_storage();
    /** @brief Returns the asset with a specified name
     * or nullptr if the asset is not loaded.
     * @param name id of the asset to be returned
     * @return pointer to the stored resource or nullptr
     */
    template<class T>
    ptr<T> get_asset(string_id name) const;
    /** @brief Registers asset with a specified name.
     * If the asset is already registered, increases its
     * reference count otherwise loads it from the disk.
     * @param name id of the asset
     */
    template<class T>
    void register_asset(string_id name);
    template<class T>
    void register_asset(string_id name, T&& asset);
    /** @brief Releases a reference to the asset. Decreases the
     * reference count of the resource and if it hits 0,
     * removes the resource from the cached assets.
     * @param name id of the asset
     */
    void release_asset(string_id name);
    /** @brief Increases the reference count of the
     * asset with specified name
     * @param name id of the asset
     * @return id of the asset
     */
    string_id clone_asset(string_id name);

  private:
    container_type cachedAssets; ///< map of the resources
    owned<asset_loader> loader;  ///< asset loader
    template<class T>
    friend class asset;
    static ptr<asset_storage> storage;

  private:
    template<class T>
    ptr<T> get_mock_asset() const;
};
template<class T>
ptr<T> asset_storage::get_mock_asset() const
{
    return static_cast<ptr<T>>(cachedAssets.at(T::get_mocked()).get());
}
template<class T>
ptr<T> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        return static_cast<ptr<T>>(res->second.get());
    }
    else
    {
        return get_mock_asset<T>();
    }
}
template<class T>
void asset_storage::register_asset(string_id name)
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        res->second->increment();
    }
    else
    {
        if(loader->resource_exists(name))
        {
            cachedAssets.insert(std::make_pair(name, loader->load_resource<T>(name)));
        }
    }
}
template<class T>
void asset_storage::register_asset(string_id name, T&& asset)
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        res->second->increment();
    }
    else
    {
        cachedAssets.insert(std::make_pair(name, std::move(asset)));
    }
}
} // namespace lemon
