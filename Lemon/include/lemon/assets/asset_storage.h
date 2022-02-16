#pragma once

#include "asset_loader.h"
#include <lemon/game/object.h>
#include <lemon/renderer/shader.h>
#include <lemon/renderer/texture.h>
#include <lemon/threads/scheduler.h>

#include <atomic>

namespace lemon {
/** Class storing the actual assets, allowing for caching assets
 * that are used in many places and managing their lifetime.
 */
class asset_storage
{
  private:
    /** @brief Stored resource. It keeps
     * a reference count. */
    struct resource
    {
        using self_type = resource;

        resource();
        resource(object* ptr);
        resource(const self_type&) = delete;
        self_type& operator=(const self_type&) = delete;
        resource(self_type&& other) noexcept;
        self_type& operator=(self_type&& other) noexcept;
        ~resource();
        void increment() noexcept;
        u32 decrement() noexcept;
        bool loaded() const noexcept;
        ptr<object> get() noexcept;
        ptr<object> get() const noexcept;
        void set(object* ptr);

      private:
        std::atomic<u32> _count; ///< reference count of the resource
        ptr<object> _stored;     ///< pointer to the resource
    };

  public:
    using self_type      = asset_storage;
    using container_type = std::unordered_map<string_id, resource>;

  public:
    /** @brief Creates the asset storage and asset loader
     * from the specified path
     * @param dataPath path to the file with the asset paths
     */
    asset_storage(const std::string& dataPath);
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
    friend class asset;
    static ptr<asset_storage> storage;
};
template<class T>
ptr<T> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        LEMON_ASSERT(!res->second.loaded());
        return static_cast<T*>(res->second.get());
    }
    LOG_ERROR("Can't find resource %s", name.get_string());
    return nullptr;
}
template<>
ptr<shader> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        if(!res->second.loaded())
        {
            return static_cast<shader*>(cachedAssets.at(string_id("mock_shader")).get());
        }
        return static_cast<shader*>(res->second.get());
    }
    LOG_ERROR("Can't find resource %s", name.get_string());
    return nullptr;
}
template<>
ptr<texture> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        if(!res->second.loaded())
        {
            return static_cast<texture*>(cachedAssets.at(string_id("mock_texture")).get());
        }
        return static_cast<texture*>(res->second.get());
    }
    LOG_ERROR("Can't find resource %s", name.get_string());
    return nullptr;
}
template<class T>
void asset_storage::register_asset(string_id name)
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        res->second.increment();
    }
    else
    {
        auto&& [pair, _] = cachedAssets.insert(std::make_pair(
            name, resource()));
        auto& val        = pair->second;
        scheduler::run([&, name]() {
            val.set(loader->load_resource<T>(name));
        });
    }
}
} // namespace lemon
