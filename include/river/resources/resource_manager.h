#pragma once

#include "resource.h"

#include <river/core/basic_types.h>
#include <unordered_map>

namespace river {
class resource_manager
{
  public:
    using self_type      = resource_manager;
    using container_type = std::unordered_map<string_id, owned<object>>;

  public:
    resource_manager();
    ~resource_manager();

    template<class T>
    static resource<T> get_resource(const string_id& name);
    template<class T>
    static void load(const std::string& path, const std::string& name);

  private:
    static resource_manager& get();
    container_type resources;
};
} // namespace river
