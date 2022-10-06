#include <world/scene_manager.h>

namespace lemon {
scene_manager::scene_manager(asset_storage& _assetStorage,
                             scheduler& _scheduler, event_queue& _eventQueue,
                             window& _window):
    _assetStorage(_assetStorage),
    _scheduler(_scheduler),
    _eventQueue(_eventQueue),
    _window(_window)
{ }

scene_manager::~scene_manager()
{ }

void scene_manager::update()
{
    currentScene->update();
}

void scene_manager::create_scene(hash_str sceneid)
{
    scenes.emplace(
        sceneid,
        new scene(sceneid, _assetStorage, _scheduler, _eventQueue, _window));
}

scene& scene_manager::load_scene(hash_str sceneid)
{
    if(currentScene)
    {
        currentScene->on_unload();
    }
    currentScene = scenes.at(sceneid).get();
    currentScene->on_load();
    return *currentScene;
}

scene& scene_manager::get_current_scene()
{
    lemon_assert(currentScene);
    return *currentScene;
}

const scene& scene_manager::get_current_scene() const
{
    lemon_assert(currentScene);
    return *currentScene;
}

scene& scene_manager::get_scene(hash_str sceneid)
{
    return *scenes.at(sceneid);
}

const scene& scene_manager::get_scene(hash_str sceneid) const
{
    return *scenes.at(sceneid);
}

bool scene_manager::is_current(hash_str sceneid) const
{
    lemon_assert(currentScene);
    return currentScene->get_nameid() == sceneid;
}
}
