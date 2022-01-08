#pragma once

#include "asset_loader.h"
#include <lemon/game/object.h>

#include <atomic>

namespace lemon {
class resource
{
    using self_type = resource;

  public:
    resource();
    resource(object* ptr);
    resource(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    resource(self_type&& other) noexcept;
    self_type& operator=(self_type&& other) noexcept;
    ~resource();
    void increment() noexcept;
    u32 decrement() noexcept;
    bool abandoned() const noexcept;
    bool loaded() const noexcept;
    ptr<object> get() noexcept;
    ptr<object> get() const noexcept;
    u32 get_count() const noexcept;

  private:
    std::atomic<u32> _count;
    std::string filepath;
    bool _loaded;
    ptr<object> _stored;
};
class asset_storage
{
    using self_type      = asset_storage;
    using container_type = std::unordered_map<string_id, resource>;

  public:
    asset_storage(const std::string& dataPath);
    ~asset_storage();

    template<class T>
    ptr<T> get_asset(string_id name) const;
    template<class T>
    bool register_asset(string_id name);
    void release_asset(string_id name);
    void clone_asset(string_id name);

  private:
    container_type cachedAssets;
    owned<asset_loader> loader;
};
template<class T>
bool asset_storage::register_asset(string_id name)
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        res->second.increment();
    }
    else
    {
        cachedAssets.insert(std::make_pair(
            name, resource(loader->load_resource<T>(name))));
    }
}
template<class T>
ptr<T> asset_storage::get_asset(string_id name) const
{
    if(auto res = cachedAssets.find(name); res != cachedAssets.end())
    {
        return res->second.get();
    }
    return nullptr;
}
} // namespace lemon
