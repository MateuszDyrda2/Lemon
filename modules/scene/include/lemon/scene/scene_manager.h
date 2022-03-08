#pragma once

#include "scene.h"

#include <queue>

namespace lemon {
class LEMON_PUBLIC scene_manager
{
  public:
    scene_manager();
    scene_manager(ptr<scene> scene);
    scene_manager(const scene_manager&) = delete;
    scene_manager& operator=(const scene_manager&) = delete;
    ~scene_manager();
    ptr<scene> push_scene(string_id name);
    void update();
    void pop_scene();
    ptr<scene> get_current_scene();
    ptr<scene> change_scene(string_id name);
    ptr<scene> change_scene(ptr<scene> scene);

  private:
    std::queue<owned<scene>> scenes;
};
} // namespace lemon
