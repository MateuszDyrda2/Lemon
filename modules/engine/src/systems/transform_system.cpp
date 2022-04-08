#include <lemon/engine/systems/transform_system.h>

#include <glm/gtx/transform.hpp>
#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec3.h>
#include <lemon/scene/scene.h>

namespace lemon {
static void dirty_on_update(entity_registry& registry, entity_handle ent)
{
    registry.emplace_or_replace<dirty>(ent);
}
transform_system::transform_system(ptr<scene> s)
{
    s->get_registry()
        .on_update<transform>()
        .connect<dirty_on_update>();
}
transform_system::~transform_system()
{
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
void transform_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    registry.sort<dirty>([&registry](const entity_handle lhs, const entity_handle rhs) {
        const auto& clhs = registry.get<transform>(lhs);
        const auto& crhs = registry.get<transform>(rhs);
        return clhs.order < crhs.order;
    });
    registry.view<dirty>().each([&registry](const entity_handle entity) {
        auto& trn          = registry.get<transform>(entity);
        auto& modelMatrix  = registry.get<model>(entity);
        modelMatrix.matrix = mat4(1.0f);
        if(trn.parent != entt::null)
        {
            modelMatrix.matrix = registry.get<model>(trn.parent).matrix;
        }
        modelMatrix.matrix = glm::translate(modelMatrix.matrix, vec3(trn.position, 0.0f));
        modelMatrix.matrix = glm::rotate(modelMatrix.matrix, trn.rotation, vec3(0.0f, 0.0f, 1.0f));
        modelMatrix.matrix = glm::scale(modelMatrix.matrix, vec3(trn.scale, 1.0f));
    });
    registry.clear<dirty>();
}
} // namespace lemon
