#include <lemon/rendering/systems/rendering_system.h>

#include <lemon/core/game.h>
#include <lemon/core/instrumentor.h>
#include <lemon/rendering/basic_renderer.h>
#include <lemon/rendering/batch_renderer.h>

#include <lemon/rendering/components/rendering_components.h>
#include <lemon/scene/components/transform_components.h>

#include <lemon/scene/scene.h>

#include <lemon/platform/window_events.h>

namespace lemon {
rendering_system::rendering_system(ptr<scene> /*s*/,
                                   event_bus& ebus):
    spriteRenderer(create_owned<basic_renderer>()),
    ebus(ebus)
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
    auto f   = static_cast<FramebufferSize*>(e);
    viewport = { f->width, f->height };
}
void rendering_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();

    auto&& mainCamera        = registry.view<main_camera_t>().front();
    auto&& [cCamera, cModel] = registry.get<camera, model>(mainCamera);

    auto newVp = vec4{ cCamera.viewport.x * viewport.x,
                       cCamera.viewport.y * viewport.y,
                       cCamera.viewport.z * viewport.x,
                       cCamera.viewport.w * viewport.y };

    rendering_context::set_viewport(newVp);
    rendering_context::clear_screen(color{ 0.5f, 0.5f, 0.5f, 1.0f });
    auto projection = glm::ortho((-1.f) * (viewport.x >> 1), (float)(viewport.x >> 1),
                                 (-1.f) * (viewport.y >> 1), (float)(viewport.y >> 1));

    glm::mat4 viewProj = projection * cModel.matrix;

    auto group = registry.group<sprite_renderer, model>();
    spriteRenderer->start_render(viewProj);
    for(auto&& [ent, sprite, mod] : group.each())
    {
        spriteRenderer->render_sprite(
            sprite.col.rgba, sprite.texCoords, sprite.text, mod.matrix);
    }
    spriteRenderer->end_render();
}
} // namespace lemon
