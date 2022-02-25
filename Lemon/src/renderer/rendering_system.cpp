#include <lemon/renderer/rendering_system.h>

#include <lemon/game.h>
#include <lemon/game/basic_components.h>
#include <lemon/game/scene.h>
#include <lemon/renderer/basic_renderer.h>
#include <lemon/renderer/batch_renderer.h>

namespace lemon {
rendering_system::rendering_system(ptr<scene> s):
    spriteRenderer(create_owned<basic_renderer>()),
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
    rendering_context::enable_blending();
}
rendering_system::~rendering_system()
{
}
void rendering_system::update(entity_registry& registry)
{
    auto [cameraComponent, cameraTransform] =
        mainCamera.get_component<camera, transform>();

    rendering_context::set_viewport(cameraComponent.viewport);
    rendering_context::clear_screen({ 0.5f, 0.5f, 0.5f, 1.0f });
    glm::mat4 viewProj = cameraComponent.projection * cameraTransform.model;

    auto group = registry.group<sprite_renderer>(entt::get<transform>);
    spriteRenderer->start_render(viewProj);
    for(auto&& [ent, sprite, tran] : group.each())
    {
        spriteRenderer->render_sprite(sprite, tran);
    }
    spriteRenderer->end_render();
}
} // namespace lemon
