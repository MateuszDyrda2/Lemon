/** @file scene_manager.h
 * @brief Class responsible for managing active scenes
 */
#pragma once

#include "scene.h"

#include <core/defines.h>
#include <core/hash_string.h>

#include <memory>
#include <unordered_map>

namespace lemon {
class serializer;
/** Class responsible for managing active scenes */
class LEMON_API scene_manager
{
  public:
    /** @brief Create a scene manager with services */
    scene_manager(asset_storage& _assetStorage, scheduler& _scheduler,
                  event_queue& _eventQueue, window& _window,
                  input& _input, message_bus& messageBus, const std::string& settingsPath);
    ~scene_manager();
    scene_manager(const scene_manager&)            = delete;
    scene_manager& operator=(const scene_manager&) = delete;

    /** @brief Create a new scene with a name
     * @param sceneId hashed scene name
     */
    void create_scene(hashstr sceneId);
    /** @brief Create a new scene from file
     * @param sceneName name of the scene
     * @param _serializer scene serializer object reference
     * @return reference to the newly created scene
     */
    scene& create_scene_defined(const std::string& sceneName, serializer& _serializer);
    /** @brief Load a pre-defined scene and return it
     * @param sceneId hashed scene name
     * @return current scene
     */
    scene& load_scene(hashstr sceneId);
    /** @brief Get current scene
     * @return current scene
     */
    scene& get_current_scene();
    /** @brief Get current scene
     * @return current scene
     */
    const scene& get_current_scene() const;
    /** @brief Get scene with nameid
     * @param sceneId hashed scene name
     * @return reference to a scene
     */
    scene& get_scene(hashstr sceneId);
    /** @brief Get scene with nameid
     * @param sceneId hashed scene name
     * @return reference to a scene
     */
    const scene& get_scene(hashstr sceneId) const;
    /** @brief Check if the scene with id is the current
     * @param sceneId hashid of the scene
     * @return true if the scene is current
     */
    bool is_current(hashstr sceneId) const;

  private:
    std::unordered_map<hashstr, std::unique_ptr<scene>> scenes;
    std::unordered_map<std::string, std::string> sceneArray;
    scene* currentScene;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
    input& _input;
    message_bus& _messageBus;
};
}
