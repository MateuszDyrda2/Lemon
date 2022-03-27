#include <lemon/engine/systems/physics_system.h>

#include <lemon/core/assert.h>
#include <lemon/core/game.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/time/clock.h>

#include <lemon/scene/scene.h>

#include <list>
#include <utility>

namespace lemon {
void physics_system::add2tree(entity_registry& registry, entity_handle ent)
{
    const transform& tr  = registry.get<const transform>(ent);
    const collider& coll = registry.get<const collider>(ent);
    tree.insert_leaf(u32(ent), get_AABB(coll, tr));
}
void physics_system::remove_from_tree(entity_registry&, entity_handle ent)
{
    tree.remove_leaf(u32(ent));
}
physics_system::physics_system(ptr<scene> s):
    gravity(-9.81f)
{
    s->get_registry()
        .on_construct<collider>()
        .connect<&physics_system::add2tree>(this);
    s->get_registry()
        .on_destroy<collider>()
        .connect<&physics_system::remove_from_tree>(this);
}
physics_system::~physics_system()
{
}
void physics_system::update(entity_registry& registry)
{
    f32 deltaTime = game::get_game_clock()->delta_time();
    for(auto&& [ent, rb] : registry.view<rigidbody>().each())
    {
        if(rb.bodyType == rigidbody::body_type::Dynamic)
        {
            apply_gravity(rb, deltaTime);
        }
    }

    auto fullGroup = registry.group<transform, collider, rigidbody>();
    //  Broad phase
    //*************************************************
    fullGroup.each([&, this](auto ent, auto& tr, auto& coll, auto& rb) {
        for(const auto& other : this->broad_collisions(u32(ent)))
        {
            const auto&& [otherTr, otherColl] = registry.get<transform, collider>(entity_handle(other));
            if(auto collision = this->collide(coll, tr, otherColl, otherTr))
            {
                if(rb.bodyType == rigidbody::body_type::Dynamic)
                {
                    auto bounciness = std::min(coll.bounciness, otherColl.bounciness);
                    auto vj         = -(1 + bounciness) * dot(rb.velocity, collision->axis);
                    auto j          = vj;
                    rb.velocity += j * collision->axis;
                    registry.emplace_or_replace<dirty>(ent);
                }
            }
        }
    });
    //*************************************************
    // update kinematics
    fullGroup.each([&, this](auto ent, auto& tr, auto& coll, auto& rb) {
        apply_drag(rb, deltaTime);
        calculate_position(rb, tr, deltaTime);
        if(!rb.freezeRotation)
        {
            f32 inertia = calculate_inertia(rb, coll);
            calculate_rotation(rb, tr, inertia, deltaTime);
        }
        registry.emplace_or_replace<dirty>(ent);
    });
    //*************************************************

    // update Axis Aligned Bounding Boxes
    //*************************************************
    registry.view<dirty, const transform, const collider>().each([this](const entity_handle ent, const transform& tr, const collider& coll) {
        tree.update_leaf(u32(ent), get_AABB(coll, tr));
    });
    //*************************************************
}
void physics_system::move_entity(entity ent, const vec2& to)
{
    auto& trns  = ent.get_component<transform>();
    auto& rb    = ent.get_component<rigidbody>();
    rb.velocity = (to - trns.position) / game::get_game_clock()->delta_time();
}
void physics_system::rotate_entity(entity ent, f32 to)
{
    auto& trns         = ent.get_component<transform>();
    auto& rb           = ent.get_component<rigidbody>();
    rb.angularVelocity = (to - trns.rotation) / game::get_game_clock()->delta_time();
}
void physics_system::add_force(entity ent, const vec2& amount)
{
    auto& rb = ent.get_component<rigidbody>();
}
void physics_system::add_torque(entity ent, f32 amount)
{
    auto& rb = ent.get_component<rigidbody>();
}
AABB physics_system::get_AABB(const collider& col, const transform& tr)
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
f32 physics_system::calculate_inertia(const rigidbody& rb, const collider& coll)
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
void physics_system::apply_gravity(rigidbody& rb, f32 deltaTime)
{
    rb.velocity.y += (gravity * rb.gravityScale) * deltaTime;
}
void physics_system::apply_drag(rigidbody& rb, f32 deltaTime)
{
    rb.velocity *= clamp(1.f - rb.linearDrag * deltaTime, 0.f, 1.f);
    rb.angularVelocity *= clamp(1.f - rb.angularDrag * deltaTime, 0.f, 1.f);
}
void physics_system::calculate_position(const rigidbody& rb, transform& tr, f32 deltaTime)
{
    //   rb.velocity += rb.force / rb.mass * deltaTime;
    tr.position += rb.velocity * deltaTime;
}
void physics_system::calculate_rotation(const rigidbody& rb, transform& tr, f32 inertia, f32 deltaTime)
{
    // rb.angularVelocity += rb.torque / inertia * deltaTime;
    tr.rotation += rb.angularVelocity * deltaTime;
}
std::list<u32> physics_system::broad_collisions(u32 entityId)
{
    return tree.query_tree(entityId);
}
std::optional<MTV> physics_system::collide(
    const collider& lhs, const transform& lhsTr,
    const collider& rhs, const transform& rhsTr) const noexcept
{
    switch((lhs.shape << 4) | rhs.shape)
    {
    // Box on box collision
    case(collider::Box << 4) | (collider::Box):
        return box_box_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
    // Box on circle collision
    case(collider::Box << 4) | (collider::Circle):
        return box_circle_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
    // Box on capsule collision
    case(collider::Box << 4) | (collider::Capsule):
        return box_capsule_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
        // Circle on box collision
    case(collider::Circle << 4) | (collider::Box):
        return box_circle_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
        // Circle on circle collision
    case(collider::Circle << 4) | (collider::Circle):
        return circle_circle_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
    // Circle on capsule collision
    case(collider::Circle << 4) | (collider::Capsule):
        return circle_capsule_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
        // Capsule on box collison
    case(collider::Capsule << 4) | (collider::Box):
        return box_capsule_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
        // Capsule on circle collison
    case(collider::Capsule << 4) | (collider::Circle):
        return circle_capsule_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
        // Capsule on capsule collison
    case(collider::Capsule << 4) | (collider::Capsule):
        return capsule_capsule_collision(
            lhs, lhsTr.position, lhsTr.rotation,
            rhs, rhsTr.position, rhsTr.rotation);
    }
}
std::optional<MTV> physics_system::box_box_collision(
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
std::optional<MTV> physics_system::box_circle_collision(
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
std::optional<MTV> physics_system::box_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
std::optional<MTV> physics_system::circle_circle_collision(
    const collider& lhs, const vec2& lhsPosition, f32,
    const collider& rhs, const vec2& rhsPosition, f32) const noexcept
{
    const auto lhsCC = lhsPosition + lhs.offset;
    const auto rhsCC = rhsPosition + rhs.offset;
    circle_shape lhsShape{ lhsCC, lhs.circle.radius };
    circle_shape rhsShape{ rhsCC, rhs.circle.radius };
    return npAlgorithm(lhsShape, rhsShape);
}
std::optional<MTV> physics_system::circle_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
std::optional<MTV> physics_system::capsule_capsule_collision(
    const collider& lhs, const vec2& lhsPosition, f32 lhsRotation,
    const collider& rhs, const vec2& rhsPosition, f32 rhsRotation) const noexcept
{
    return {};
}
void physics_system::rotate_box(vec2& a, vec2& b, vec2& c, vec2& d, f32 degrees) const noexcept
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
