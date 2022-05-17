#pragma once

#include "entity.h"
#include "system.h"

#include <lemon/core/math/vec2.h>
#include <lemon/core/service_registry.h>
#include <lemon/core/time/game_clock.h>

#include <functional>
#include <map>
#include <vector>

namespace lemon {
struct update_stage
{
    static struct frame_begin_t
    {
    } frameBegin;
    static struct update_begin_t
    {
    } updateBegin;
    static struct physics_t
    {
    } physics;
    static struct update_end_t
    {
    } updateEnd;
    static struct frame_end_t
    {
    } frameEnd;
};

class LEMON_PUBLIC scene
{
  public:
    scene(string_id name, service_registry& globalRegistry, entity_registry&& registry);
    scene(string_id name, service_registry& global_registry);
    ~scene();
    template<class T, class Stage, class... Args>
    ptr<scene> add_system(Stage updateStage, size_type order, Args&&... args);

    void scene_load();
    void scene_start();

    void frame_begin();
    void update_begin();
    void physics_update();
    void update_end();
    void frame_end();

    void scene_end();
    void scene_unload();

    const string_id& get_id() const { return name; }

    /*
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
        */

  private:
    string_id name;
    entity_registry registry;
    service_registry globalRegistry;

    std::vector<owned<system>> systems;
    std::map<size_type, ptr<system>> onFrameBeginMap;
    std::map<size_type, ptr<system>> onUpdateBeginMap;
    std::map<size_type, ptr<system>> onPhysicsMap;
    std::map<size_type, ptr<system>> onUpdateEndMap;
    std::map<size_type, ptr<system>> onFrameEndMap;

  private:
    friend class scene_attachment;
    void push2map(ptr<system> s, update_stage::frame_begin_t, size_type order);
    void push2map(ptr<system> s, update_stage::update_begin_t, size_type order);
    void push2map(ptr<system> s, update_stage::physics_t, size_type order);
    void push2map(ptr<system> s, update_stage::update_end_t, size_type order);
    void push2map(ptr<system> s, update_stage::frame_end_t, size_type order);
};
template<class T, class Stage, class... Args>
inline ptr<scene> scene::add_system(Stage updateStage, size_type order, Args&&... args)
{
    const auto index = systems.size();
    systems.push_back(create_owned<T>(globalRegistry, std::forward<Args>(args)...));
    push2map(systems[index].get());
    return this;
}
} // namespace lemon
