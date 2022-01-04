#pragma once

#include "object.h"
#include "scene.h"

#include <river/service/service.h>

#include <queue>

namespace river {
class scene_manager : public service
{
  public:
    scene_manager();
    ptr<scene> push_scene(string_id name);
    void update();
    void pop_scene();
    scene* get_current_scene();

  private:
    std::queue<owned<scene>> scenes;
    ptr<rendering_context> context;
};
} // namespace river