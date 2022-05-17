#include <lemon/scene/systems/transform_system.h>

#include <glm/gtx/transform.hpp>
#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec3.h>
#include <lemon/scene/scene.h>

namespace lemon {
static void dirty_on_update(entity_registry& registry, entity_handle ent)
{
    registry.emplace_or_replace<dirty_t>(ent);
}
transform_system::transform_system(service_registry& globalRegistry):
    system(globalRegistry)
{ }
transform_system::~transform_system()
{ }
void transform_system::on_scene_load(entity_registry& registry)
{
    registry.on_update<transform>().connect<dirty_on_update>();
}
void transform_system::on_scene_unload(entity_registry& registry)
{
    registry.on_update<transform>().disconnect<dirty_on_update>();
}
void transform_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    registry.sort<dirty_t>([&registry](const entity_handle lhs, const entity_handle rhs) {
        const auto& clhs = registry.get<transform>(lhs);
        const auto& crhs = registry.get<transform>(rhs);
        return clhs.order < crhs.order;
    });
    registry.view<dirty_t>().each([&registry](const entity_handle ent) {
        auto&& [trn, mdl] = registry.get<transform, model>(ent);
        mdl.matrix        = mat4(1.0f);
        if(trn.parent != entt::null)
        {
            mdl.matrix = registry.get<model>(trn.parent).matrix;
        }
        mdl.matrix = glm::translate(mdl.matrix, vec3(trn.position, 0.0f));
        mdl.matrix = glm::rotate(mdl.matrix, trn.rotation, vec3(0.0f, 0.0f, 1.0f));
        mdl.matrix = glm::scale(mdl.matrix, vec3(trn.scale, 1.0f));
    });
    registry.clear<dirty_t>();
}
void transform_system::translate(entity ent, const vec2& by)
{
    ent.patch_component<transform>([&](auto& t) { t.position += by; });
}
void transform_system::rotate(entity ent, f32 by)
{
    ent.patch_component<transform>([&](auto& t) { t.rotation += by; });
}
void transform_system::scale(entity ent, const vec2& by)
{
    ent.patch_component<transform>([&](auto& t) { t.scale += by; });
}
} // namespace lemon
