#include <lemon/physics/systems/collision_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
AABB calculate_AABB(const collider& col, const transform& tr);
std::optional<MTV> collide(SAT& sat,
                           const collider& lhs, const transform& lhsTr,
                           const collider& rhs, const transform& rhsTr) noexcept;
std::optional<MTV> box_box_collision(SAT& sat,
                                     const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
                                     const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;
std::optional<MTV> box_circle_collision(SAT& sat,
                                        const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
                                        const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;
std::optional<MTV> circle_circle_collision(SAT& sat,
                                           const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
                                           const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) noexcept;

void collision_system::add2tree(entity_registry& registry, entity_handle ent)
{
    const auto&& [tr, coll] = registry.get<transform, collider>(ent);
    tree.insert_leaf(u32(ent), calculate_AABB(coll, tr));
}
void collision_system::remove_from_tree(entity_registry& /* registy*/, entity_handle ent)
{
    tree.remove_leaf(u32(ent));
}
collision_system::collision_system(service_registry& globalRegistry):
    system(globalRegistry), sch(globalRegistry.get_service<scheduler>())
{
}
collision_system::~collision_system()
{
}
void collision_system::on_scene_load(entity_registry& registry)
{
    registry.on_construct<collider>().connect<&collision_system::add2tree>(this);
    registry.on_destroy<collider>().connect<&collision_system::remove_from_tree>(this);
}
void collision_system::on_scene_unload(entity_registry& registry)
{
    registry.on_construct<collider>().disconnect<&collision_system::add2tree>(this);
    registry.on_destroy<collider>().disconnect<&collision_system::remove_from_tree>(this);
}
void collision_system::on_update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();

    registry.clear<collision_m, trigger_m>();

    auto group = registry.group<transform, collider, rigidbody>();
    group.each([&, this](auto ent, auto& tr, auto& coll, auto& rb) {
        tree.update_leaf(u32(ent), calculate_AABB(coll, tr));
    });

    group.each([&, this](auto ent, auto& tr, auto& coll, auto& rb) {
        for(const auto& other : tree.query_tree(u32(ent)))
        {
            const auto otherEnt = entity_handle(other);
            const auto&& [otherTr, otherColl] =
                registry.get<const transform, const collider>(otherEnt);
            if(auto collis = collide(npAlgorithm, coll, tr, otherColl, otherTr))
            {
                if(registry.all_of<is_trigger_t>(otherEnt))
                {
                    registry.emplace<trigger_m>(registry.create(), ent, otherEnt);
                }
                else
                {
                    registry.emplace<collision_m>(registry.create(), ent, otherEnt, collis.value());
                }
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
        lemon_assert(0);
    }
}
std::optional<MTV> collide(SAT& sat,
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
            sat,
            lhs, aCenter, aRotation,
            rhs, bCenter, bRotation);
    // Box on circle collision
    case boxCircleCollision:
        return box_circle_collision(
            sat,
            lhs, aCenter, aRotation,
            rhs, bCenter, bRotation);
        // Circle on box collision
    case circleBoxCollision:
        return box_circle_collision(
            sat,
            rhs, bCenter, bRotation,
            lhs, aCenter, aRotation);
        // Circle on circle collision
    case circleCircleCollision:
        return circle_circle_collision(
            sat,
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
std::optional<MTV> box_box_collision(SAT& sat,
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
        return sat(lhsShape, rhsShape, SAT::rotated);
    }
    else
    {
        const box_shape lhsShape = { { lhsA, lhsB, lhsC, lhsD } };
        const box_shape rhsShape = { { rhsA, rhsB, rhsC, rhsD } };
        return sat(lhsShape, rhsShape, SAT::axis_aligned);
    }
}
std::optional<MTV> circle_circle_collision(SAT& sat,
                                           const collider& lhs, const vec2& lhsPosition, f32,
                                           const collider& rhs, const vec2& rhsPosition, f32) noexcept
{
    const circle_shape lhsShape{ lhsPosition, lhs.circle.radius };
    const circle_shape rhsShape{ rhsPosition, rhs.circle.radius };
    return sat(lhsShape, rhsShape);
}
std::optional<MTV> box_circle_collision(SAT& sat,
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
        return sat(lhsShape, rhsShape);
    }
    else
    {
        box_shape lhsShape{ { lhsA, lhsB, lhsC, lhsD } };
        circle_shape rhsShape{ rhsPosition, rhs.circle.radius };
        return sat(lhsShape, rhsShape);
    }
}
} // lemon
