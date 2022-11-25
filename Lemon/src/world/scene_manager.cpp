#include <world/scene_manager.h>

namespace lemon {
scene_manager::scene_manager(asset_storage& _assetStorage,
                             scheduler& _scheduler, event_queue& _eventQueue,
                             window& _window, input& _input, message_bus& _messageBus):
    _assetStorage(_assetStorage),
    _scheduler(_scheduler),
    _eventQueue(_eventQueue),
    _window(_window),
    _input(_input),
    _messageBus(_messageBus)
{ }

scene_manager::~scene_manager()
{ }

void scene_manager::create_scene(hashstr sceneid)
{
    scenes.emplace(
        sceneid,
        new scene(sceneid, _assetStorage, _scheduler, _eventQueue, _window, _input, _messageBus));
}

scene& scene_manager::load_scene(hashstr sceneid)
{
    if (currentScene)
    {
        //        currentScene->on_unload();
    }
    currentScene = scenes.at(sceneid).get();
    //   currentScene->on_load();
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

scene& scene_manager::get_scene(hashstr sceneid)
{
    return *scenes.at(sceneid);
}

const scene& scene_manager::get_scene(hashstr sceneid) const
{
    return *scenes.at(sceneid);
}

bool scene_manager::is_current(hashstr sceneid) const
{
    lemon_assert(currentScene);
    return currentScene->get_nameid() == sceneid;
}
}
