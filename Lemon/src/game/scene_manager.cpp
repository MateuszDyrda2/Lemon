#include <lemon/game/scene_manager.h>

namespace lemon {
scene_manager::scene_manager()
{
    LOG_MESSAGE("Scene Manager created");
}
ptr<scene> scene_manager::push_scene(string_id name)
{
    LOG_MESSAGE("New scene %s", name.get_string());
    auto ptr = scenes.emplace(create_owned<scene>(name)).get();
    if(scenes.size() == 1)
    {
        event_dispatcher.send(string_id("OnSceneBegin"), get_current_scene()->get_id());
    }

    return ptr;
}
void scene_manager::update()
{
    if(scenes.size())
        scenes.front()->update();
}
void scene_manager::pop_scene()
{
    event_dispatcher.send(string_id("onSceneEnd"), get_current_scene()->get_id());
    scenes.pop();
    event_dispatcher.send(string_id("OnSceneBegin"), get_current_scene()->get_id());
}
ptr<scene> scene_manager::get_current_scene()
{
    return scenes.front().get();
}
ptr<scene> scene_manager::change_scene(string_id name)
{
    LOG_MESSAGE("New scene %s", name.get_string());
    if(scenes.size())
    {
        event_dispatcher.send(string_id("onSceneEnd"), get_current_scene()->get_id());
        scenes.pop();
    }
    auto current = scenes.emplace(create_owned<scene>(name)).get();
    event_dispatcher.send(string_id("onSceneBegin"), current->get_id());
    return current;
}
} // namespace lemon
