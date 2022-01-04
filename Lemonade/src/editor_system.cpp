#include <RiverEditor/editor_system.h>

editor_system::editor_system(ptr<lemon::scene> sc):
    mainCamera(sc->get_main_camera())
{
}
editor_system::~editor_system()
{
}
void editor_system::update(entity_registry& registry)
{
    auto& t = mainCamera.get_component<transform>();
    t.model = glm::translate(glm::mat4(1.f), t.position);
    t.model = glm::rotate(t.model, t.rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    t.model = glm::scale(t.model, t.scale);
}