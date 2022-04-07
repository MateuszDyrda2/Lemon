#include <lemon/engine/systems/rendering_system.h>

#include <lemon/core/game.h>
#include <lemon/core/instrumentor.h>
#include <lemon/rendering/basic_renderer.h>
#include <lemon/rendering/batch_renderer.h>
#include <lemon/scene/components/rendering_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

#include <lemon/platform/window_events.h>

namespace lemon {
rendering_system::rendering_system(ptr<scene> s,
                                   event_bus& ebus):
    spriteRenderer(create_owned<basic_renderer>()),
    mainCamera(s->get_main_camera()), ebus(ebus)
{
    ebus.sink(string_id("FramebufferSize")) += this;
    rendering_context::enable_blending();
}
rendering_system::~rendering_system()
{
    ebus.sink(string_id("FramebufferSize")) -= this;
}
void rendering_system::on_event(event* e)
{
    auto f = static_cast<FramebufferSize*>(e);
    mainCamera.change_component<camera>(
        glm::vec4{ 0.0f, 0.0f, f->width, f->height },
        glm::ortho(
            (-1.f) * (f->width >> 1), (float)(f->width >> 1),
            (-1.f) * (f->height >> 1), (float)(f->height >> 1)));
}
void rendering_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    auto [cameraComponent, cameraTransform] =
        mainCamera.get_component<camera, transform>();

    rendering_context::set_viewport(cameraComponent.viewport);
    rendering_context::clear_screen(color{ 0.5f, 0.5f, 0.5f, 1.0f });
    glm::mat4 viewProj = cameraComponent.projection * cameraTransform.model;

    auto group = registry.group<sprite_renderer>(entt::get<transform>);
    spriteRenderer->start_render(viewProj);
    for(auto&& [ent, sprite, tran] : group.each())
    {
        spriteRenderer->render_sprite(
            sprite.col.rgba, sprite.texCoords, sprite.text, tran.model);
    }
    spriteRenderer->end_render();
}
} // namespace lemon
