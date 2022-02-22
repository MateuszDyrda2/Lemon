#pragma once

#include "object.h"
#include "scene.h"

#include <lemon/events/dispatcher.h>

#include <queue>

namespace lemon {
class scene_manager
{
  public:
    scene_manager();
    scene_manager(ptr<scene> scene);
    ptr<scene> push_scene(string_id name);
    void update();
    void pop_scene();
    ptr<scene> get_current_scene();
    ptr<scene> change_scene(string_id name);

  private:
    std::queue<owned<scene>> scenes;
    dispatcher event_dispatcher;
};
} // namespace lemon
