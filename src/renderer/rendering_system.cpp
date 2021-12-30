#include <river/renderer/rendering_system.h>

#include <river/game/basic_components.h>

namespace river {
rendering_system::rendering_system(entity_registry& registry)
{
    auto group     = registry.group<sprite_renderer>(entt::get<transform>);
    context        = create_owned<opengl_context>();
    spriteRenderer = create_owned<batch_renderer>();
}
rendering_system::~rendering_system()
{
}
void rendering_system::render(entity_registry& registry, entity mainCamera)
{
    auto& [cameraComponent, cameraTransform] =
        mainCamera.get_component<camera, transform>();
    glm::mat4 viewProj = cameraComponent.projection * cameraTransform.model;

    auto group = registry.group<sprite_renderer>(entt::get<transform>);
    for(auto&& [ent, sprite, tran] : group.each())
    {
        spriteRenderer->render_sprite(viewProj, context.get(), sprite, tran);
    }
    spriteRenderer->end_render(viewProj, context.get());
}
} // namespace river
