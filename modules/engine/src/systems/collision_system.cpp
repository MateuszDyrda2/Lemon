#include <lemon/engine/systems/collision_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/scene/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
AABB calculate_AABB(const collider& col, const transform& tr);
collision_system::collision_system(ptr<scene> s, clock& clk, scheduler& sch):
    clk(clk), sch(sch)
{
}
collision_system::~collision_system()
{
}
void collision_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    // TODO: use different tag / store aabbs somewhere
    registry.view<dirty, const transform, const collider>().each(
        [this](auto ent, auto& tr, auto& coll) {
            tree.update_leaf(u32(ent), calculate_AABB(coll, tr));
        });

    auto deltaTime = clk.delta_time();

    auto group = registry.group<transform, collider, rigidbody>();
    sch.for_each(group.begin(), group.end(),
                 [&, this](auto ent, auto& tr, auto& coll, auto& rb) {
                     for(const auto& other : tree.query_tree(ent))
                     {
                         const auto&& [otherTr, otherColl] = registry.get<transform, collider>(entity_handle(other));
                         if(auto collision = this->collide(coll, tr, otherColl, otherTr))
                         {
                             auto bounciness = std::min(coll.bounciness, otherColl.bounciness);
                             auto vj         = -(1 + bounciness) * dot(rb.velocity, collision->axis);
                             auto j          = vj;
                             rb.velocity += j * collision->axis;
                         }
                     }
                 });
}
AABB calculate_AABB(const collider& col, const transform& tr)
{
    const auto realPos = tr.position + col.offset;
    switch(col.shape)
    {
    case collider::Box:
        return AABB{ { realPos - col.box.hSize }, { realPos + col.box.hSize } };
    case collider::Circle:
        return AABB{ { realPos - col.circle.radius }, { realPos + col.circle.radius } };
    case collider::Capsule:
        return AABB{ { realPos.x - col.capsule.radius, realPos.y - (col.capsule.hHeight + col.capsule.radius) },
                     { realPos.x + col.capsule.radius, realPos.y + (col.capsule.hHeight + col.capsule.radius) } };
    default:
        LEMON_ASSERT(0);
    }
}
std::optional<MTV> collision_system::collide(
    const collider& lhs, const transform& lhsTr,
    const collider& rhs, const transform& rhsTr) noexcept
{
    static constexpr size_type boxBoxCollision       = (collider::Box << 4) | (collider::Box);
    static constexpr size_type boxCircleCollision    = (collider::Box << 4) | (collider::Circle);
    static constexpr size_type circleCircleCollision = (collider::Circle << 4) | (collider::Circle);
    static constexpr size_type circleBoxCollision    = (collider::Circle << 4) | (collider::Box);

    // TODO: Calculate position and rotation in world - not rel to parent
    const auto aCenter   = lhsTr.position + lhs.offset;
    const auto bCenter   = rhsTr.position + rhs.offset;
    const auto aRotation = lhsTr.rotation;
    const auto bRotation = rhsTr.rotation;

    switch((lhs.shape << 4) | rhs.shape)
    {
    // Box on box collision
    case boxBoxCollision:
        return box_box_collision(
            lhs, aCenter, aRotation,
            rhs, bCenter, bRotation);
    // Box on circle collision
    case boxCircleCollision:
        return box_circle_collision(
            lhs, aCenter, aRotation,
            rhs, bCenter, bRotation);
        // Circle on box collision
    case circleBoxCollision:
        return box_circle_collision(
            rhs, bCenter, bRotation,
            lhs, aCenter, aRotation);
        // Circle on circle collision
    case circleCircleCollision:
        return circle_circle_collision(
            lhs, aCenter, aRotation,
            rhs, bCenter, bRotation);
    }
}
void rotate_box(vec2& a, vec2& b, vec2& c, vec2& d, f32 degrees) noexcept
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
std::optional<MTV> collision_system::box_box_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept
{
    auto lhsA = lhsPosition + vec2(-lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsB = lhsPosition + vec2(lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsC = lhsPosition + vec2(lhs.box.hSize.x, lhs.box.hSize.y);
    auto lhsD = lhsPosition + vec2(-lhs.box.hSize.x, lhs.box.hSize.y);

    auto rhsA = rhsPosition + vec2(-rhs.box.hSize.x, -rhs.box.hSize.y);
    auto rhsB = rhsPosition + vec2(rhs.box.hSize.x, -rhs.box.hSize.y);
    auto rhsC = rhsPosition + vec2(rhs.box.hSize.x, rhs.box.hSize.y);
    auto rhsD = rhsPosition + vec2(-rhs.box.hSize.x, rhs.box.hSize.y);

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
std::optional<MTV> collision_system::circle_circle_collision(
    const collider& lhs, const vec2& lhsPosition, f32,
    const collider& rhs, const vec2& rhsPosition, f32) noexcept
{
    const circle_shape lhsShape{ lhsPosition, lhs.circle.radius };
    const circle_shape rhsShape{ rhsPosition, rhs.circle.radius };
    return npAlgorithm(lhsShape, rhsShape);
}
std::optional<MTV> collision_system::box_circle_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32) noexcept
{
    auto lhsA = lhsPosition + vec2(-lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsB = lhsPosition + vec2(lhs.box.hSize.x, -lhs.box.hSize.y);
    auto lhsC = lhsPosition + vec2(lhs.box.hSize.x, lhs.box.hSize.y);
    auto lhsD = lhsPosition + vec2(-lhs.box.hSize.x, lhs.box.hSize.y);

    if((int)lhsRotation % 90)
    {
        rotate_box(lhsA, lhsB, lhsC, lhsD, lhsRotation);
        box_shape lhsShape{ { lhsA, lhsB, lhsC, lhsD } };
        circle_shape rhsShape{ rhsPosition, rhs.circle.radius };
        return npAlgorithm(lhsShape, rhsShape);
    }
    else
    {
        box_shape lhsShape{ { lhsA, lhsB, lhsC, lhsD } };
        circle_shape rhsShape{ rhsPosition, rhs.circle.radius };
        return npAlgorithm(lhsShape, rhsShape);
    }
}
} // lemon