#pragma once

#include "entity.h"
#include "system_registry.h"
#include "world_system.h"

#include <lemon/core/math/vec2.h>

#include <functional>
#include <map>
#include <vector>

namespace lemon {
struct update_stage
{
    static struct frame_begin_t
    {
    } frameBegin;
    static struct physics_begin_t
    {
    } physicsBegin;
    static struct physics_t
    {
    } physics;
    static struct physics_end_t
    {
    } physicsEnd;
    static struct frame_end_t
    {
    } frameEnd;
};

class LEMON_PUBLIC scene
{
  public:
    scene(string_id name, entity_registry&& registry);
    scene(string_id name);
    ~scene();
    template<class T, class UpdateStage, class... Args>
    ptr<scene> add_system(UpdateStage updateStage, size_type updateOrder, Args&&... args);
    void initialize();
    void update(system_registry& sregistry);
    void begin();
    void set_main_camera(entity mc);

    const string_id& get_id() const { return name; }

    entity add_entity(string_id name);
    entity add_entity(string_id name, entity parent);
    entity add_entity(string_id name, const vec2& position,
                      const vec2& scale, f32 rotation);
    entity add_entity(string_id name, const vec2& position,
                      const vec2& scale, f32 rotation, entity parent);
    entity clone_entity(entity ent, string_id name);
    void remove_entity(entity& ent);
    entity_registry& get_registry() { return registry; }
    entity get_main_camera() const { return mainCamera; }

  private:
    string_id name;
    entity_registry registry;
    entity mainCamera;
    std::vector<owned<world_system>> systems;
    std::map<size_type, ptr<world_system>> onFrameBeginUpdate;
    std::map<size_type, ptr<world_system>> onPhysicsBeginUpdate;
    std::map<size_type, ptr<world_system>> onPhysicsUpdate;
    std::map<size_type, ptr<world_system>> onPhysicsEndUpdate;
    std::map<size_type, ptr<world_system>> onFrameEndUpdate;

  private:
    void scene_load(system_registry& sregistry);
    void frame_begin(system_registry& sregistry);
    void physics_begin(system_registry& sregistry);
    void physics(system_registry& sregistry);
    void physics_end(system_registry& sregistry);
    void frame_end(system_registry& sregistry);
    void scene_unload(system_registry& sregistry);

  private:
    void push2map(ptr<system> s, size_type updateOrder, update_stage::frame_begin_t);
    void push2map(ptr<system> s, size_type updateOrder, update_stage::physics_begin_t);
    void push2map(ptr<system> s, size_type updateOrder, update_stage::physics_t);
    void push2map(ptr<system> s, size_type updateOrder, update_stage::physics_end_t);
    void push2map(ptr<system> s, size_type updateOrder, update_stage::frame_end_t);
};
template<class T, class UpdateStage, class... Args>
inline ptr<scene> add_system(UpdateStage updateStage, size_type updateOrder, Args&&... args)
{
    const auto index = systems.size();
    systems.push_back(create_owned<T>(this, std::forward<Args>(args)...));
    push2map(systems[index].get(), updateOrder);
    return this;
}
} // namespace lemon
