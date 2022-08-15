#pragma once

#include "scene.h"

#include <core/defines.h>
#include <core/hash_string.h>

#include <memory>
#include <unordered_map>

namespace lemon {
class LEMON_API scene_manager
{
  public:
    scene_manager(asset_storage& _assetStorage, scheduler& _scheduler,
                  event_queue& _eventQueue, window& _window);
    ~scene_manager();
    scene_manager(const scene_manager&) = delete;
    scene_manager& operator=(const scene_manager&) = delete;

    void update();
    void create_scene(hash_str sceneId);
    scene& load_scene(hash_str sceneId);
    scene& get_current_scene();
    const scene& get_current_scene() const;
    scene& get_scene(hash_str sceneId);
    const scene& get_scene(hash_str sceneId) const;
    bool is_current(hash_str sceneId) const;

  private:
    std::unordered_map<hash_str, std::unique_ptr<scene>> scenes;
    scene* currentScene;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
};
}
