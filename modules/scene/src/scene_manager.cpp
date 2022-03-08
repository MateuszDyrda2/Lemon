#include <lemon/scene/scene_manager.h>

#include <lemon/core/game.h>

namespace lemon {
scene_manager::scene_manager()
{
    LOG_MESSAGE("Scene Manager created");
}
scene_manager::scene_manager(ptr<scene> scene)
{
    LOG_MESSAGE("New scene %s", scene->get_id().get_string());
    scenes.emplace(scene);
}
scene_manager::~scene_manager()
{
    LOG_MESSAGE("Scene manager destroyed");
}
ptr<scene> scene_manager::push_scene(string_id name)
{
    LOG_MESSAGE("New scene %s", name.get_string());
    auto ptr = scenes.emplace(create_owned<scene>(name)).get();

    return ptr;
}
void scene_manager::update()
{
    if(scenes.size())
        scenes.front()->update();
}
void scene_manager::pop_scene()
{
    scenes.pop();
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
        scenes.pop();
    }
    auto current = scenes.emplace(create_owned<scene>(name)).get();
    return current;
}
ptr<scene> scene_manager::change_scene(ptr<scene> scene)
{
    LOG_MESSAGE("New scene %s", scene->get_id().get_string());
    if(scenes.size())
    {
        scenes.pop();
    }
    auto current = scenes.emplace(scene).get();
    return current;
}
} // namespace lemon
