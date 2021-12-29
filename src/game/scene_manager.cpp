#pragma once

#include <river/game/scene_manager.h>

namespace river {
scene_manager::scene_manager():
    object("scene_manager")
{
}
void scene_manager::push_scene(const std::string& name)
{
    scenes.push(create_owned<scene>(name));
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
