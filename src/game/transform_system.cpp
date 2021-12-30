#include <river/game/transform_system.h>

#include <glm/gtx/transform.hpp>

namespace river {
transform_system::transform_system(entity_registry& registry)
{
    auto group = registry.group<dirty>(entt::get<transform>);
    registry.on_update<transform>()
        .connect<&entt::registry::emplace_or_replace<dirty>>(&registry);
}
transform_system::~transform_system()
{
}
void transform_system::update(entity_registry& registry)
{
    registry.sort<dirty>([&registry](const string_id lhs, const string_id rhs) {
        const auto& clhs = registry.get<transform>(lhs);
        const auto& crhs = registry.get<transform>(rhs);
        return clhs.order < crhs.order;
    });
    registry.view<dirty>().each([&registry](const string_id entity) {
        auto& trn      = registry.get<transform>(entity);
        glm::mat4 base = glm::mat4(1.0f);
        if(trn.parent)
        {
            base = trn.parent->model;
        }
        base      = glm::translate(base, trn.position);
        base      = glm::rotate(base, trn.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        base      = glm::scale(base, trn.scale);
        trn.model = base;
    });
    registry.clear<dirty>();
}
} // namespace river
