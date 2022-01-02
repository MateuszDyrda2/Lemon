#include <river/game/scene_manager.h>

#include <river/service/services.h>

namespace river {
scene_manager::scene_manager():
    context(services::get<rendering_context>())
{
    LOG_MESSAGE("Scene Manager created");
}
void scene_manager::push_scene(string_id name)
{
    LOG_MESSAGE("New scene %s", name.get_string().c_str());
    scenes.push(create_owned<scene>(name, context));
}
void scene_manager::update(float deltaTime)
{
    if(scenes.size())
        scenes.front()->update(deltaTime);
}
void scene_manager::pop_scene()
{
    scenes.pop();
}
scene* scene_manager::get_current_scene()
{
    return scenes.front().get();
}
} // namespace river
