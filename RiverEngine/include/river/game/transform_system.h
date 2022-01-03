#pragma once

#include <river/game/basic_components.h>

#include <concepts>
#include <entt/entt.hpp>
#include <type_traits>

namespace river {
class scene;
class transform_system
{
  public:
    template<class F>
    static void for_each_child(entity ent, F&& callable) requires(
        std::is_invocable_v<F, entity>);
    template<class F>
    static void for_each_child_r(entity ent, F&& callable) requires(
        std::is_invocable_v<F, entity>);

    transform_system(ptr<scene> s);
    ~transform_system();
    void update(entity_registry& registry);
    friend class scene;
};
template<class F>
void transform_system::for_each_child(entity ent, F&& callable) requires(
    std::is_invocable_v<F, entity>)
{
    auto registry = ent.get_registry();
    auto child    = ent.get_component<transform>().first;
    while(child != entt::null)
    {
        entity c(registry, child, false);
        callable(c);
        child = c.get_component<transform>().next;
    }
}
template<class F>
void transform_system::for_each_child_r(entity ent, F&& callable) requires(
    std::is_invocable_v<F, entity>)
{
    auto registry = ent.get_registry();
    auto child    = ent.get_component<transform>().first;
    while(child != entt::null)
    {
        entity c(registry, child, false);
        callable(c);
        for_each_child_r(c, callable);
        child = c.get_component<transform>().next;
    }
}
} // namespace river