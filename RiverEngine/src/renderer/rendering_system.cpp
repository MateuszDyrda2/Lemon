#include <river/renderer/rendering_system.h>

#include <river/game/basic_components.h>
#include <river/game/scene.h>
#include <river/service/services.h>

namespace river {
rendering_system::rendering_system(ptr<scene> s,
                                   ptr<rendering_context> context):
    spriteRenderer(create_owned<batch_renderer>()),
    context(context), mainCamera(s->add_entity(string_id("MainCamera"))),
    handler(services::get<event_handler>())
{
    mainCamera.add_component<camera>();
    vpResize = handler->subscribe<int, int>(
        string_id("FramebufferSize"),
        [this](int width, int height) {
            LOG_MESSAGE("Viewport resize: %dx%d", width, height);
            mainCamera.change_component<camera>(
                glm::vec4{ 0.0f, 0.0f, width, height },
                glm::ortho(width / -2.f, width / 2.f, height / -2.f, height / 2.f));
        });
    context->enable_blending();
}
rendering_system::~rendering_system()
{
}
void rendering_system::render(entity_registry& registry)
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
} // namespace river
