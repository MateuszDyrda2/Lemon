#include <lemon/physics/physics_engine.h>

#include <lemon/core/assert.h>
#include <lemon/core/math/math.h>

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
AABB physics_engine::get_AABB(const collider& coll, const vec2& position) const noexcept
{
    const auto realPos = position + coll.offset;
    switch(coll.shape)
    {
    case collider::Box:
        return AABB{ { realPos - coll.box.hSize }, { realPos + coll.box.hSize } };
    case collider::Circle:
        return AABB{ { realPos - coll.circle.radius }, { realPos + coll.circle.radius } };
    case collider::Capsule:
        return AABB{ { realPos.x - coll.capsule.radius, realPos.y - (coll.capsule.hHeight + coll.capsule.radius) },
                     { realPos.x + coll.capsule.radius, realPos.y + (coll.capsule.hHeight + coll.capsule.radius) } };
    default:
        LEMON_ASSERT(0);
    }
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
f32 physics_engine::calculate_inertia(const rigidbody& rb, const collider& coll)
{
    switch(coll.shape)
    {
    case collider::Box:
        return (1.f / 12.f) * rb.mass
               * ((2 * coll.box.hSize.x) * (2 * coll.box.hSize.x) + (2 * coll.box.hSize.y) * (2 * coll.box.hSize.y));
    case collider::Circle:
        return (0.5f) * rb.mass * (coll.circle.radius * coll.circle.radius);
    // https://www.gamedev.net/forums/topic/686143-calculating-the-moment-of-inertia-of-a-2d-capsule/5331404/
    case collider::Capsule:
        return (0.5f * rb.mass)
               * (9.f * (coll.capsule.radius * coll.capsule.radius)
                  + 4.f * (coll.capsule.hHeight * coll.capsule.hHeight)
                  + 1.5f * (coll.capsule.radius * coll.capsule.hHeight));
    }
}
bool physics_engine::collide(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
    switch((lhs.shape << 4) | rhs.shape)
    {
    // Box on box collision
    case(collider::Box << 4) | (collider::Box):
        return box_box_collision(lhs, lhsPosition, rhs, rhsPosition);
    // Box on circle collision
    case(collider::Box << 4) | (collider::Circle):
        return box_circle_collision(lhs, lhsPosition, rhs, rhsPosition);
    // Box on capsule collision
    case(collider::Box << 4) | (collider::Capsule):
        return box_capsule_collision(lhs, lhsPosition, rhs, rhsPosition);
    // Circle on box collision
    case(collider::Circle << 4) | (collider::Box):
        return box_circle_collision(rhs, rhsPosition, lhs, lhsPosition);
    // Circle on circle collision
    case(collider::Circle << 4) | (collider::Circle):
        return circle_circle_collision(lhs, lhsPosition, rhs, rhsPosition);
    // Circle on capsule collision
    case(collider::Circle << 4) | (collider::Capsule):
        return circle_capsule_collision(lhs, lhsPosition, rhs, rhsPosition);
    // Capsule on box collison
    case(collider::Capsule << 4) | (collider::Box):
        return box_capsule_collision(rhs, rhsPosition, lhs, lhsPosition);
    // Capsule on circle collison
    case(collider::Capsule << 4) | (collider::Circle):
        return circle_capsule_collision(rhs, rhsPosition, lhs, lhsPosition);
    // Capsule on capsule collison
    case(collider::Capsule << 4) | (collider::Capsule):
        return capsule_capsule_collision(lhs, lhsPosition, rhs, rhsPosition);
    }
}
bool physics_engine::box_box_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
    const auto lhsRealPos = lhsPosition + lhs.offset;
    const auto rhsRealPos = rhsPosition + rhs.offset;
    return lhsRealPos.x - lhs.box.hSize.x
               >= rhsRealPos.x + rhs.box.hSize.x
           && lhsRealPos.y - lhs.box.hSize.y
                  >= rhsRealPos.y + rhs.box.hSize.y;
}
bool physics_engine::box_circle_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
    const auto lhsRealPos = lhsPosition + lhs.offset;
    const auto rhsRealPos = rhsPosition + rhs.offset;
    const auto closestX   = clamp(rhsRealPos.x,
                                  lhsRealPos.x - lhs.box.hSize.x,
                                  lhsRealPos.x + lhs.box.hSize.x);
    const auto closestY   = clamp(rhsRealPos.y,
                                  lhsRealPos.y - lhs.box.hSize.y,
                                  lhsRealPos.y + lhs.box.hSize.y);

    const auto distanceX   = rhsRealPos.x - closestX;
    const auto distanceY   = rhsRealPos.y - closestY;
    const auto distanceSqr = (distanceX * distanceX)
                             + (distanceY * distanceY);
    return distanceSqr <= (rhs.circle.radius * rhs.circle.radius);
}
bool physics_engine::box_capsule_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
}
bool physics_engine::circle_circle_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
    const auto lhsRealPos  = lhsPosition + lhs.offset;
    const auto rhsRealPos  = rhsPosition + rhs.offset;
    const auto distanceX   = rhsRealPos.x - lhsRealPos.x;
    const auto distanceY   = rhsRealPos.y - lhsRealPos.y;
    const auto distanceSqr = (distanceX * distanceX)
                             + (distanceY * distanceY);
    return distanceSqr
           <= ((lhs.circle.radius * rhs.circle.radius)
               + (lhs.circle.radius * rhs.circle.radius));
}
bool physics_engine::circle_capsule_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
}
bool physics_engine::capsule_capsule_collision(
    const collider& lhs, const vec2& lhsPosition,
    const collider& rhs, const vec2& rhsPosition) const noexcept
{
}

} // namespace lemon
