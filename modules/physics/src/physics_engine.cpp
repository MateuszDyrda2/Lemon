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
void physics_engine::apply_gravity(rigidbody& rb, f32 deltaTime)
{
    rb.velocity.y += (gravity * rb.gravityScale) * deltaTime;
}
void physics_engine::apply_drag(rigidbody& rb, f32 deltaTime)
{
    rb.velocity *= clamp(1.f - rb.linearDrag * deltaTime, 0.f, 1.f);
    rb.angularVelocity *= clamp(1.f - rb.angularDrag * deltaTime, 0.f, 1.f);
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
    //   rb.velocity += rb.force / rb.mass * deltaTime;
    pos += rb.velocity * deltaTime;
}
void physics_engine::calculate_rotation(rigidbody& rb, f32& rotation, f32 inertia, f32 deltaTime)
{
    // rb.angularVelocity += rb.torque / inertia * deltaTime;
    rotation += rb.angularVelocity * deltaTime;
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
void physics_engine::insert_collider(u32 entityId, const AABB& box)
{
    tree.insert_leaf(entityId, box);
}
void physics_engine::remove_collider(u32 entityId)
{
    tree.remove_leaf(entityId);
}
void physics_engine::update_collider(u32 entityId, const AABB& box)
{
    tree.update_leaf(entityId, box);
}
std::list<u32> physics_engine::broad_collisions(u32 entityId)
{
    return tree.query_tree(entityId);
}
std::optional<MTV> physics_engine::collide(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    switch((lhs.shape << 4) | rhs.shape)
    {
    // Box on box collision
    case(collider::Box << 4) | (collider::Box):
        return box_box_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    // Box on circle collision
    case(collider::Box << 4) | (collider::Circle):
        return box_circle_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    // Box on capsule collision
    case(collider::Box << 4) | (collider::Capsule):
        return box_capsule_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    // Circle on box collision
    case(collider::Circle << 4) | (collider::Box):
        return box_circle_collision(
            rhs, rhsPosition, lhsRotation,
            lhs, lhsPosition, rhsRotation);
    // Circle on circle collision
    case(collider::Circle << 4) | (collider::Circle):
        return circle_circle_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    // Circle on capsule collision
    case(collider::Circle << 4) | (collider::Capsule):
        return circle_capsule_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    // Capsule on box collison
    case(collider::Capsule << 4) | (collider::Box):
        return box_capsule_collision(
            rhs, rhsPosition, lhsRotation,
            lhs, lhsPosition, rhsRotation);
    // Capsule on circle collison
    case(collider::Capsule << 4) | (collider::Circle):
        return circle_capsule_collision(
            rhs, rhsPosition, lhsRotation,
            lhs, lhsPosition, rhsRotation);
    // Capsule on capsule collison
    case(collider::Capsule << 4) | (collider::Capsule):
        return capsule_capsule_collision(
            lhs, lhsPosition, lhsRotation,
            rhs, rhsPosition, rhsRotation);
    }
}
std::optional<MTV> physics_engine::box_box_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    const auto lhsCC = lhsPosition + lhs.offset;
    const auto rhsCC = rhsPosition + rhs.offset;

    auto lhsA = lhsCC + vec2(-lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsB = lhsCC + vec2(lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsC = lhsCC + vec2(lhs.box.hSize.x, lhs.box.hSize.y);
    auto lhsD = lhsCC + vec2(-lhs.box.hSize.x, lhs.box.hSize.y);

    auto rhsA = rhsCC + vec2(-rhs.box.hSize.x, -rhs.box.hSize.y);
    auto rhsB = rhsCC + vec2(rhs.box.hSize.x, -rhs.box.hSize.y);
    auto rhsC = rhsCC + vec2(rhs.box.hSize.x, rhs.box.hSize.y);
    auto rhsD = rhsCC + vec2(-rhs.box.hSize.x, rhs.box.hSize.y);

    if(int(lhsRotation) % 90 || int(rhsRotation) % 90)
    {
        rotate_box(lhsA, lhsB, lhsC, lhsD, lhsRotation);
        rotate_box(rhsA, rhsB, rhsC, rhsD, rhsRotation);
        const box_shape lhsShape = { { lhsA, lhsB, lhsC, lhsD } };
        const box_shape rhsShape = { { rhsA, rhsB, rhsC, rhsD } };
        return npAlgorithm(lhsShape, rhsShape, SAT::rotated);
    }
    else
    {
        const box_shape lhsShape = { { lhsA, lhsB, lhsC, lhsD } };
        const box_shape rhsShape = { { rhsA, rhsB, rhsC, rhsD } };
        return npAlgorithm(lhsShape, rhsShape, SAT::axis_aligned);
    }
}
std::optional<MTV> physics_engine::box_circle_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32) const noexcept
{
    const auto lhsCC = lhsPosition + lhs.offset;
    const auto rhsCC = rhsPosition + rhs.offset;

    auto lhsA = lhsCC + vec2(-lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsB = lhsCC + vec2(lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsC = lhsCC + vec2(lhs.box.hSize.x, lhs.box.hSize.y);
    auto lhsD = lhsCC + vec2(-lhs.box.hSize.x, lhs.box.hSize.y);

    if((int)lhsRotation % 90)
    {
        rotate_box(lhsA, lhsB, lhsC, lhsD, lhsRotation);
        box_shape lhsShape{ { lhsA, lhsB, lhsC, lhsD } };
        circle_shape rhsShape{ rhsCC, rhs.circle.radius };
        return npAlgorithm(lhsShape, rhsShape);
    }
    else
    {
        box_shape lhsShape{ { lhsA, lhsB, lhsC, lhsD } };
        circle_shape rhsShape{ rhsCC, rhs.circle.radius };
        return npAlgorithm(lhsShape, rhsShape);
    }
}
std::optional<MTV> physics_engine::box_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
std::optional<MTV> physics_engine::circle_circle_collision(
    const collider& lhs, const vec2& lhsPosition, f32,
    const collider& rhs, const vec2& rhsPosition, f32) const noexcept
{
    const auto lhsCC = lhsPosition + lhs.offset;
    const auto rhsCC = rhsPosition + rhs.offset;
    circle_shape lhsShape{ lhsCC, lhs.circle.radius };
    circle_shape rhsShape{ rhsCC, rhs.circle.radius };
    return npAlgorithm(lhsShape, rhsShape);
}
std::optional<MTV> physics_engine::circle_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
std::optional<MTV> physics_engine::capsule_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
void physics_engine::rotate_box(vec2& a, vec2& b, vec2& c, vec2& d, f32 degrees) const noexcept
{
    const auto rads = radians(degrees);
    const auto csin = sin(rads);
    const auto ccos = cos(rads);

    a = { ccos * a.x - csin * a.y,
          csin * a.x + ccos * a.y };
    b = { ccos * b.x - csin * b.y,
          csin * b.x + ccos * b.y };
    c = { ccos * c.x - csin * c.y,
          csin * c.x + ccos * c.y };
    d = { ccos * d.x - csin * d.y,
          csin * d.x + ccos * d.y };
}
} // namespace lemon
