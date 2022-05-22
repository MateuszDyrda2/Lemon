#pragma once

#include "entity.h"
#include "system.h"

#include <lemon/core/math/vec2.h>
#include <lemon/core/service_registry.h>
#include <lemon/core/time/game_clock.h>
#include <lemon/threading/scheduler.h>

#include <functional>
#include <map>
#include <vector>

namespace lemon {
struct update_stage
{
    static struct frame_begin_t
    {
    } frameBegin;
    static struct update_t
    {
    } updateBegin;
    static struct physics_t
    {
    } physics;
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
    void physics_update();
    void update();
    void frame_end();

    void scene_end();
    void scene_unload();

    const string_id& get_id() const { return name; }

  private:
    string_id name;
    entity_registry registry;
    service_registry globalRegistry;
    job physicsJob;
    double accumulator;
    game_clock& clk;

    std::vector<owned<system>> systems;
    std::map<size_type, ptr<system>> onFrameBeginMap;
    std::map<size_type, ptr<system>> onPhysicsMap;
    std::map<size_type, ptr<system>> onUpdateMap;
    std::map<size_type, ptr<system>> onFrameEndMap;

  private:
    friend class scene_attachment;
    void push2map(ptr<system> s, update_stage::frame_begin_t, size_type order);
    void push2map(ptr<system> s, update_stage::update_t, size_type order);
    void push2map(ptr<system> s, update_stage::physics_t, size_type order);
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
