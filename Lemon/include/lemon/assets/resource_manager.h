#pragma once

#include "resource.h"

#include <future>
#include <lemon/containers/buffer.h>
#include <lemon/core/basic_types.h>
#include <lemon/service/service.h>

#include <unordered_map>

namespace lemon {
class job_system;
class resource_manager : public service
{
  public:
    using self_type      = resource_manager;
    using container_type = std::unordered_map<string_id, owned<object>>;
    template<class T>
    using future_type = std::future<T>;

  public:
    resource_manager();
    ~resource_manager();

    template<class T>
    asset<T> get_resource(string_id name);
    template<class T, class... Args>
    asset<T> load(string_id name, Args&&... args);
    void unload(string_id name);

  private:
    container_type resources;
    ptr<job_system> jobSystem;

  private:
    buffer<byte> read_file(const std::string& path);
    future_type<buffer<byte>> read_async(const std::string& path);
};
template<class T>
asset<T> resource_manager::get_resource(string_id name)
{
    return asset<T>(static_cast<T*>(resources.at(name).get()));
}
template<class T, class... Args>
asset<T> resource_manager::load(string_id name, Args&&... args)
{
    auto inserted = resources.insert(std::make_pair(name, create_owned<T>(name, std::forward<Args>(args)...)));
    LEMON_ASSERT(inserted.second);
    return asset<T>(static_cast<T*>(((*inserted.first).second.get())));
}

} // namespace lemon
