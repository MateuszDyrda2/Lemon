#pragma once

#include "object.h"
#include "scene.h"

#include <queue>

namespace river {
class scene_manager : public object
{
  public:
    scene_manager();
    void push_scene(const std::string& name);
    void update(float deltaTime) { }
    void pop_scene();
    scene* get_current_scene();

  private:
    std::queue<owned<scene>> scenes;
};
} // namespace river
