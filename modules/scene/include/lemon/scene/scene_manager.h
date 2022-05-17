#pragma once

#include "scene.h"
#include <lemon/core/service.h>

#include <unordered_map>

namespace lemon {
class service_registry;
class LEMON_PUBLIC scene_manager : public service
{
  public:
    LEMON_REGISTER_SERVICE(scene_manager);

  public:
    scene_manager(service_registry& globalRegistry);
    scene_manager(service_registry& globalRegistry, ptr<scene> scene);
    ~scene_manager();

    void update();

    void create_scene(string_id sceneId);
    scene& load_scene(string_id sceneId);
    scene& get_current_scene();
    scene& get_scene(string_id sceneId);
    bool is_scene(string_id sceneId);

  private:
    service_registry& globalRegistry;
    std::unordered_map<string_id, scene> scenes;
    ptr<scene> currentScene;
};
} // namespace lemon
