#include <lemon/physics/physics_engine.h>

namespace lemon {
physics_engine::physics_engine():
    gravity(-0.981f)
{
}
physics_engine::~physics_engine()
{
}
void physics_engine::apply_gravity(vec2& force, f32 mass)
{
    force.y += mass * gravity;
}
void physics_engine::calculate_position(vec2 force, f32 mass,
                                        vec2& vel, vec2& pos, f32 deltaTime)
{
    vec2 linAcc = force / mass;
    vel += linAcc * deltaTime;
    pos += vel * deltaTime;
}
void physics_engine::calculate_rotation(f32 torque, f32 inertia,
                                        f32& angularVelocity, f32& rotation,
                                        f32 deltaTime)
{
    f32 angAcc = torque / inertia;
    angularVelocity += angAcc * deltaTime;
    rotation += angularVelocity * deltaTime;
}
f32 physics_engine::calculate_inertia(
    f32 momentOfIntertia, f32 mass, vec4 bounds)
{
    return momentOfIntertia * mass
           * ((bounds.z - bounds.x) * (bounds.z - bounds.x)
              + (bounds.w - bounds.y) * (bounds.w - bounds.y));
}

} // namespace lemon
