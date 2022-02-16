#include <lemon/renderer/rendering_system.h>

#include <lemon/game.h>
#include <lemon/game/basic_components.h>
#include <lemon/game/scene.h>

namespace lemon {
rendering_system::rendering_system(ptr<scene> s):
    spriteRenderer(create_owned<batch_renderer>()),
    context(game::get_rendering_context()),
    mainCamera(s->get_main_camera()),
    vpResize(
        string_id("FramebufferSize"),
        [this](int width, int height) {
            mainCamera.change_component<camera>(
                glm::vec4{ 0.0f, 0.0f, width, height },
                glm::ortho(
                    (-1.f) * (width >> 1), (float)(width >> 1),
                    (-1.f) * (height >> 1), (float)(height >> 1)));
        })
{
    context->enable_blending();
}
rendering_system::~rendering_system()
{
}
void rendering_system::update(entity_registry& registry)
{
    auto [cameraComponent, cameraTransform] =
        mainCamera.get_component<camera, transform>();

    context->set_viewport(cameraComponent.viewport);
    context->clear_screen({ 0.5f, 0.5f, 0.5f, 1.0f });
    glm::mat4 viewProj = cameraComponent.projection * cameraTransform.model;

    auto group = registry.group<sprite_renderer>(entt::get<transform>);
    for(auto&& [ent, sprite, tran] : group.each())
    {
        spriteRenderer->render_sprite(viewProj, context, sprite, tran);
    }
    spriteRenderer->end_render(viewProj, context);
}
} // namespace lemon
