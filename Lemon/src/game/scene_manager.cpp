#include <lemon/game/scene_manager.h>

#include <lemon/service/services.h>

namespace lemon {
scene_manager::scene_manager():
    context(services::get<rendering_context>())
{
    LOG_MESSAGE("Scene Manager created");
}
ptr<scene> scene_manager::push_scene(string_id name)
{
    LOG_MESSAGE("New scene %s", name.get_string());
    return scenes.emplace(create_owned<scene>(name)).get();
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
scene* scene_manager::get_current_scene()
{
    return scenes.front().get();
}
} // namespace lemon
