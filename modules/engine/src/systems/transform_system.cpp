#include <lemon/engine/systems/transform_system.h>

#include <glm/gtx/transform.hpp>
#include <lemon/core/math/math.h>
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
void transform_system::update(entity_registry& registry)
{
    registry.sort<dirty>([&registry](const entity_handle lhs, const entity_handle rhs) {
        const auto& clhs = registry.get<transform>(lhs);
        const auto& crhs = registry.get<transform>(rhs);
        return clhs.order < crhs.order;
    });
    registry.view<dirty>().each([&registry](const entity_handle entity) {
        auto& trn = registry.get<transform>(entity);
        trn.model = mat4(1.0f);
        if(trn.parent != entt::null)
        {
            trn.model = registry.get<transform>(trn.parent).model;
        }
        trn.model = glm::translate(trn.model, vec3(trn.position, 0.0f));
        trn.model = glm::rotate(trn.model, trn.rotation, vec3(0.0f, 0.0f, 1.0f));
        trn.model = glm::scale(trn.model, vec3(trn.scale, 1.0f));
    });
    registry.clear<dirty>();
}
} // namespace lemon
