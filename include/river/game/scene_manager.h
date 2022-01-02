#pragma once

#include "object.h"
#include "scene.h"

#include <queue>

namespace river {
class scene_manager
{
  public:
    scene_manager(ptr<rendering_context> context);
    void push_scene(string_id name);
    void update(float deltaTime);
    void pop_scene();
    scene* get_current_scene();

  private:
    std::queue<owned<scene>> scenes;
    ptr<rendering_context> context;
};
} // namespace river
