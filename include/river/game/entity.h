#pragma once

#include <entt/entity/registry.hpp>
#include <river/core/basic_types.h>
#include <river/core/string_id.h>

namespace river {
using entity_registry = entt::basic_registry<string_id>;
class entity
{
  public:
    entity(ptr<entity_registry> registry, string_id name);
    template<class T, class... Args>
    void add_component(Args&&... args);
    template<class... Args>
    auto& get_component();
    template<class... Args>
    const auto& get_component() const;
    template<class T>
    bool has_component() const;
    template<class... Args>
    bool has_all_of() const;
    template<class... Args>
    bool has_any_of() const;
    template<class T>
    void remove_component();

    string_id get_handle() const { return handle; }

  private:
    string_id handle;
    ptr<entity_registry> registry;
};
template<class T, class... Args>
inline void entity::add_component(Args&&... args)
{
    registry->emplace<T>(handle, std::forward<Args>(args)...);
}
template<class... Args>
inline auto& entity::get_component()
{
    return registry->get<Args...>(handle);
}
template<class... Args>
inline const auto& entity::get_component() const
{
    return registry->get<Args...>(handle);
}
template<class T>
inline bool entity::has_component() const
{
    return registry->any_of<T>(handle);
}
template<class... Args>
inline bool entity::has_all_of() const
{
    return registry->all_of<Args...>(handle);
}
template<class... Args>
inline bool entity::has_any_of() const
{
    return registry->any_of<Args..>(handle);
}
template<class T>
inline void entity::remove_component()
{
    registry->remove<T>(handle);
}
} // namespace river
