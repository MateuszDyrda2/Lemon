#pragma once

#include <lemon/scene/system.h>

#include <lemon/physics/physics_engine.h>

namespace lemon {
class scene;
class LEMON_PUBLIC physics_system : public system
{
  public:
    physics_system(ptr<scene> s);
    ~physics_system();

    void update(entity_registry& registry) override;

    static void move_entity(entity ent, const vec2& to);
    static void rotate_entity(entity ent, f32 to);
    static void add_force(entity ent, const vec2& amount);
    static void add_torque(entity ent, f32 amount);

  private:
    physics_engine pEngine;

  private:
    void add2tree(entity_registry& registry, entity_handle ent);
    void remove_from_tree(entity_registry& registry, entity_handle ent);
};
} // namespace lemon