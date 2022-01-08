#pragma once

#include <lemon/containers/buffer.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/string_id.h>

#include <unordered_map>

namespace lemon {
class asset_loader
{
  public:
    using self_type      = asset_loader;
    using container_type = std::unordered_map<string_id, std::string>;

  public:
    asset_loader(const std::string& dataPath);
    ~asset_loader();
    template<class T>
    ptr<T> load_resource(string_id name);
    bool resource_exists(string_id name) const noexcept;

  private:
    container_type resourcePaths;

  private:
    buffer<byte> load_from_file(const std::string& path);
};
template<class T>
ptr<T> asset_loader::load_resource(string_id name)
{
    if(resourcePaths.contains(name))
    {
        ptr<T> resource = new T(std::move(load_from_file(resourcePaths[name])));
        return resource;
    }
}
} // namespace lemon
