#pragma once

#include <entt/entity/registry.hpp>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>

namespace lemon {
using entity_registry = entt::registry;
using entity_handle   = entt::entity;
class LEMON_PUBLIC entity
{
  public:
    entity() = default;
    entity(ptr<entity_registry> registry, entity_handle name);
    template<class T, class... Args>
    decltype(auto) add_component(Args&&... args);
    template<class T, class... Args>
    void emplace_or_replace(Args&&... args);
    template<class T, class... Args>
    void change_component(Args&&... args);
    template<class T, class F>
    void patch_component(F&& callable);
    template<class... Args>
    decltype(auto) get_component();
    template<class... Args>
    decltype(auto) get_component() const;
    template<class T>
    bool has_component() const;
    template<class... Args>
    bool has_all_of() const;
    template<class... Args>
    bool has_any_of() const;
    template<class T>
    void remove_component();
    static void destroy(entity ent);
    static entity clone(entity orginal, string_id name);

    entity_handle get_handle() const { return handle; }
    ptr<entity_registry> get_registry() { return registry; }

    explicit operator bool() const
    {
        return registry != nullptr;
    }

  private:
    ptr<entity_registry> registry{};
    entity_handle handle{ entt::null };
};
template<class T, class... Args>
inline decltype(auto) entity::add_component(Args&&... args)
{
    return registry->emplace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class... Args>
inline void entity::emplace_or_replace(Args&&... args)
{
    registry->emplace_or_replace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class... Args>
inline void entity::change_component(Args&&... args)
{
    registry->replace<T>(handle, std::forward<Args>(args)...);
}
template<class T, class F>
inline void entity::patch_component(F&& callable)
{
    registry->patch<T>(handle, callable);
}
template<class... Args>
inline decltype(auto) entity::get_component()
{
    return registry->get<Args...>(handle);
}
template<class... Args>
inline decltype(auto) entity::get_component() const
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
    return registry->any_of<Args...>(handle);
}
template<class T>
inline void entity::remove_component()
{
    registry->remove<T>(handle);
}
} // namespace lemon
