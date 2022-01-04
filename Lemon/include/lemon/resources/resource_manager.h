#pragma once

#include "resource.h"

#include <lemon/core/basic_types.h>
#include <lemon/service/service.h>

#include <unordered_map>

namespace lemon {
class resource_manager : public service
{
  public:
    using self_type      = resource_manager;
    using container_type = std::unordered_map<string_id, owned<object>>;

  public:
    resource_manager();
    ~resource_manager();

    template<class T>
    resource<T> get_resource(string_id name);
    template<class T, class... Args>
    resource<T> load(string_id name, Args&&... args);
    void unload(string_id name);

  private:
    container_type resources;
};
template<class T>
resource<T> resource_manager::get_resource(string_id name)
{
    return resource<T>(static_cast<T*>(resources.at(name).get()));
}
template<class T, class... Args>
resource<T> resource_manager::load(string_id name, Args&&... args)
{
    auto inserted = resources.insert(std::make_pair(name, create_owned<T>(name, std::forward<Args>(args)...)));
    LEMON_ASSERT(inserted.second);
    return resource<T>(static_cast<T*>(((*inserted.first).second.get())));
}

} // namespace lemon
