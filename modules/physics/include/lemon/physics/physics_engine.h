#pragma once

#include "physics_components.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

namespace lemon {
class LEMON_PUBLIC physics_engine
{
  public:
    physics_engine();
    ~physics_engine();
    void apply_gravity(rigidbody& rb);
    void calculate_position(rigidbody& rb, vec2& pos, f32 deltaTime);
    void calculate_rotation(rigidbody& rb, f32& rotation, f32 inertia, f32 deltaTime);
    f32 calculate_inertia(const rigidbody& rb, const box_collider& bc);
    f32 calculate_inertia(const rigidbody& rb, const circle_collider& cc);

  private:
    f32 gravity;
};
} // namespace lemon
