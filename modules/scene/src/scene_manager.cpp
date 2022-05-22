#include <lemon/scene/scene_manager.h>

#include <lemon/core/service_registry.h>

namespace lemon {
using namespace std;
scene_manager::scene_manager(service_registry& globalRegistry):
    globalRegistry(globalRegistry)
{
    LOG_MESSAGE("Scene Manager created");
}
scene_manager::scene_manager(service_registry& globalRegistry, ptr<scene> scene):
    globalRegistry(globalRegistry)
{
    LOG_MESSAGE("New scene %s", scene->get_id().get_string());
    scenes.emplace(scene);
}
scene_manager::~scene_manager()
{
    LOG_MESSAGE("Scene manager destroyed");
}
void scene_manager::frame_begin()
{
    lemon_assert(currentScene != nullptr);
    currentScene->frame_begin();
}
void scene_manager::physics_update()
{
    lemon_assert(currentScene != nullptr);
    currentScene->physics_update();
}
void scene_manager::update()
{
    lemon_assert(currentScene != nullptr);
    currentScene->update();
}
void scene_manager::frame_end()
{
    lemon_assert(currentScene != nullptr);
    currentScene->frame_end();
}
void scene_manager::create_scene(string_id sceneId)
{
    lemon_assert(!scenes.contains(sceneId));
    scenes.emplace(make_pair(sceneId, scene(sceneId, globalRegistry)));
}
scene& scene_manager::load_scene(string_id sceneId)
{
    if(currentScene)
    {
        currentScene->scene_unload();
    }
    lemon_assert(scenes.contains(sceneId));
    currentScene = &scenes.at(sceneId);
    LOG_MESSAGE("New scene %s", sceneId.get_string());

    currentScene->scene_load();
    return *currentScene;
}
scene& scene_manager::get_current_scene()
{
    lemon_assert(currentScene != nullptr);
    return *currentScene;
}
scene& scene_manager::get_scene(string_id sceneId)
{
    lemon_assert(scenes.contains(sceneId));
    return scenes.at(sceneId);
}
} // namespace lemon
