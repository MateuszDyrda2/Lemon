#pragma once

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
    void apply_gravity(vec2& force, f32 mass);
    void calculate_position(vec2 force, f32 mass,
                            vec2& vel, vec2& pos, f32 deltaTime);
    void calculate_rotation(f32 torque, f32 inertia,
                            f32& angularVelocity, f32& rotation,
                            f32 deltaTime);
    f32 calculate_inertia(f32 momentOfIntertia, f32 mass, vec4 bounds);

  private:
    f32 gravity;
};
} // namespace lemon
