#pragma once

#include <entt/entt.hpp>

namespace lemon {
class entity
{
  public:
    entity() = default;
    entity(entt::entity ent, entt::registry* p_world);
    template<class T, class... Args>
    decltype(auto) emplace(Args&&... args);
    template<class T, class... Args>
    void emplace_or_replace(Args&&... args);
    template<class T, class... Args>
    void replace_component(Args&&... args);
    template<class T, class F>
    void patch_component(F&& callable);
    template<class... Args>
    decltype(auto) get_components();
    template<class... Args>
    decltype(auto) get_components() const;
    template<class T>
    bool has_component() const;
    template<class... Args>
    bool has_all_of() const;
    template<class... Args>
    bool has_any_of() const;
    template<class T>
    void remove_component();
    explicit operator bool() const
    {
        return p_world != nullptr;
    }

  private:
    entt::entity ent;
    entt::registry* p_world{};
};
entity::entity(entt::entity ent, entt::registry* p_world):
    ent(ent), p_world(p_world)
{ }
template<class T, class... Args>
decltype(auto) entity::emplace(Args&&... args)
{
    return p_world->emplace<T>(ent, std::forward<Args>(args)...);
}
template<class T, class... Args>
void entity::emplace_or_replace(Args&&... args)
{
    p_world->emplace_or_replace<T>(ent, std::forward<Args>(args)...);
}
template<class T, class... Args>
void entity::replace_component(Args&&... args)
{
    p_world->replace<T>(ent, std::forward<Args>(args)...);
}
template<class T, class F>
void entity::patch_component(F&& callable)
{
    p_world->patch<T>(ent, std::forward<F>(callable));
}
template<class... Args>
decltype(auto) entity::get_components()
{
    return p_world->get<Args...>(ent);
}
template<class... Args>
decltype(auto) entity::get_components() const
{
    return p_world->get<Args...>(ent);
}
template<class T>
bool entity::has_component() const
{
    return p_world->any_of<T>(ent);
}
template<class... Args>
bool entity::has_all_of() const
{
    return p_world->all_of<Args...>(ent);
}
template<class... Args>
bool entity::has_any_of() const
{
    return p_world->any_of<Args...>(ent);
}
template<class T>
void entity::remove_component()
{
    p_world->remove<T>(ent);
}

} // namespace lemon
