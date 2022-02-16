#pragma once

#include "object.h"
#include "scene.h"

#include <queue>

namespace lemon {
class scene_manager
{
  public:
    scene_manager(ptr<rendering_context> context);
    ptr<scene> push_scene(string_id name);
    void update();
    void pop_scene();
    scene* get_current_scene();

  private:
    std::queue<owned<scene>> scenes;
    ptr<rendering_context> context;
};
} // namespace lemon
