#pragma once

#include <concepts>
#include <entt/entt.hpp>
#include <lemon/scene/basic_components.h>
#include <lemon/scene/scene.h>
#include <lemon/scene/system.h>
#include <type_traits>

namespace lemon {
class scene;
class LEMON_PUBLIC transform_system : public system
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
    void update(entity_registry& registry) override;
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
} // namespace lemon