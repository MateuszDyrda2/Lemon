#include <lemon/engine/systems/transform_system.h>

#include <glm/gtx/transform.hpp>
#include <lemon/core/math/math.h>
#include <lemon/scene/scene.h>

namespace lemon {
static void dirty_on_update(entity_registry& registry, entity_handle ent)
{
    auto child = registry.get<transform>(ent).first;
    while(child != entt::null)
    {
        registry.patch<transform>(child);
        child = registry.get<transform>(child).next;
    }
}
transform_system::transform_system(ptr<scene> s)
{
    /* s->get_registry()
         .on_update<transform>()
         .connect<dirty_on_update>(); */
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
        auto& trn      = registry.get<transform>(entity);
        glm::mat4 base = mat4(1.0f);
        if(trn.parent != entt::null)
        {
            auto& parentTrn = registry.get<transform>(trn.parent);
            base            = parentTrn.model;
        }
        base      = glm::translate(base, trn.position);
        base      = glm::rotate(base, trn.rotation, vec3(0.0f, 0.0f, 1.0f));
        base      = glm::scale(base, trn.scale);
        trn.model = base;
    });
    registry.clear<dirty>();
}
} // namespace lemon
