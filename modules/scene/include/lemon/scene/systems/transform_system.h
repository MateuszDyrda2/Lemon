#pragma once

#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>
#include <lemon/scene/system.h>

#include <entt/entt.hpp>

#include <concepts>
#include <list>
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

    static void translate(entity ent, const vec2& by);
    static void rotate(entity ent, f32 by);
    static void scale(entity ent, const vec2& by);

    transform_system(service_registry& globalRegistry);
    ~transform_system();

    void on_scene_load(entity_registry& registry) override;
    void on_update(entity_registry& registry) override;
    void on_scene_unload(entity_registry& registry) override;
};
template<class F>
void transform_system::for_each_child(entity ent, F&& callable) requires(
    std::is_invocable_v<F, entity>)
{
    auto registry = ent.get_registry();
    auto child    = ent.get_component<transform>().first;
    while(child != entt::null)
    {
        entity c(registry, child);
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
        entity c(registry, child);
        callable(c);
        for_each_child_r(c, callable);
        child = c.get_component<transform>().next;
    }
}
} // namespace lemon