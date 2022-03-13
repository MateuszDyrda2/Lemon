#include <lemon/physics/physics_engine.h>

namespace lemon {
physics_engine::physics_engine():
    gravity(-9.81f)
{
}
physics_engine::~physics_engine()
{
}
void physics_engine::apply_gravity(rigidbody& rb)
{
    rb.force.y += rb.mass * gravity * rb.gravityScale;
}
void physics_engine::calculate_position(rigidbody& rb, vec2& pos, f32 deltaTime)
{
    rb.velocity += rb.force / rb.mass * deltaTime;
    pos += rb.velocity * deltaTime;
    rb.force = vec2(0);
}
void physics_engine::calculate_rotation(rigidbody& rb, f32& rotation, f32 inertia, f32 deltaTime)
{
    rb.angularVelocity += rb.torque / inertia * deltaTime;
    rotation += rb.angularVelocity * deltaTime;
    rb.torque = 0.f;
}
f32 physics_engine::calculate_inertia(const rigidbody& rb, const box_collider& bc)
{
    // 1/12m(w^2 + h^2)
    static constexpr f32 moi = 1.f / 12.f;
    return moi * rb.mass * ((bc.size.x * bc.size.x) + (bc.size.y * bc.size.y));
}
f32 physics_engine::calculate_inertia(const rigidbody& rb, const circle_collider& cc)
{
    // 1/2mr^2
    static constexpr f32 moi = 0.5f;
    return moi * rb.mass * rb.mass * (cc.radius * cc.radius);
}
} // namespace lemon
