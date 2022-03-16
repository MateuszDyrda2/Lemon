#include <lemon/engine/systems/physics_system.h>

#include <lemon/core/game.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/time/clock.h>
#include <lemon/scene/basic_components.h>
#include <lemon/scene/scene.h>

#include <list>
#include <utility>

namespace lemon {
physics_system::physics_system(ptr<scene>)
{
}
physics_system::~physics_system()
{
}
void physics_system::update(entity_registry& registry)
{
    // Fill all AABB's positions
    //*************************************************
    registry.view<transform, collider_component>().each(
        [](const entity_handle ent, transform& trns, collider_component& coll) {
            auto&& [min, max] = coll.collider->get_extends(trns.position);
            coll.aabb.min     = min;
            coll.aabb.max     = max;
        });
    //*************************************************

    // Sort the AABB's using min extend
    //*************************************************
    static u32 axis = 0; // start with x axis
    registry.sort<collider_component>([](const collider_component& lhs, const collider_component& rhs) {
        return lhs.aabb.min[axis] < rhs.aabb.min[axis];
    });
    //*************************************************

    // calculate max variance axis and complete broad phase
    //*************************************************
    vec2 centerSum;
    vec2 centerSumSq;
    auto&& colliderView = registry.view<collider_component>();
    for(auto iter = colliderView.begin(); iter != std::prev(colliderView.end()); ++iter)
    {
        auto& collider = colliderView.get<collider_component>(*iter);
        auto& aabb     = collider.aabb;
        // sum the center coordinates
        vec2 center = aabb.max - aabb.min;
        centerSum += center;
        centerSumSq += vec2(center.x * center.x, center.y * center.y);

        for(auto iter2 = std::next(iter); iter2 != colliderView.end(); ++iter2)
        {
            auto& collider2 = colliderView.get<collider_component>(*iter2);
            auto& aabb2     = collider2.aabb;
            if(aabb.max[axis] < aabb2.min[axis]) break;
            // A possible collision
            // if(detect_collision(collider.collider.get(), collider2.collider.get()))
            //{
            //    // detected collision
            //}
        }
    }
    // calculate variance
    centerSum /= colliderView.size();
    centerSumSq /= colliderView.size();
    vec2 variance = centerSumSq - (centerSum * centerSum);
    axis          = u32(variance.x > variance.y);
    //*************************************************

    // update kinematics
    f32 deltaTime = game::get_game_clock()->delta_time();
    auto group    = registry.group<rigidbody, box_collider>(entt::get<transform>);
    for(auto&& [ent, rb, bc, tr] : group.each())
    {
        pEngine.apply_gravity(rb);
        pEngine.calculate_position(
            rb, tr.position, deltaTime);
        f32 inertia = pEngine.calculate_inertia(rb, bc);
        pEngine.calculate_rotation(
            rb, tr.rotation, inertia, deltaTime);
        registry.emplace_or_replace<dirty>(ent);
    }
    // TODO:    Implement collision detection and collision resolving
#if 0  // collision
    // COLLISIONS
    // 1. Sort on the x axis by AABB's x coordinate
    struct AABB
    {
        vec2 min, max;
    };

    registry.sort<
        box_collider>(
        +[](const entity_handle lhs, const entity_handle rhs, entity_registry& registry) {
            auto& bclhs = registry.get<box_collider>(lhs);
            auto& bcrhs = registry.get<box_collider>(rhs);
            auto& trlhs = registry.get<transform>(lhs);
            auto& trrhs = registry.get<transform>(rhs);
            AABB a      = {
                vec2(trlhs.position.x + bclhs.center.x - bclhs.size.x * 0.5f),
                vec2(trlhs.position.y + bclhs.center.y - bclhs.size.y * 0.5f)
            };
            AABB b = {
                vec2(trrhs.position.x + bcrhs.center.x - bcrhs.size.x * 0.5f),
                vec2(trrhs.position.y + bcrhs.center.y - bcrhs.size.y * 0.5f)
            };

            return a.min.x < b.max.x;
        },
        entt::insertion_sort(), registry);

    // 2. Create a list of ovelapped AABB's
    using namespace std;
    list<box_collider> potential;
    list<pair<entity_handle, entity_handle>> overlaps;

    for(auto&& [ent, bc] : registry.view<box_collider>().each())
    {
        for(auto iter = potential.begin(); iter != potential.end();)
        {

            if(bc.bounds.x > iter->be.y)
            {
                bounds.erase(iter++);
            }
            else
            {
                overlaps.push_back(
                    make_pair(iter->handle, ent));
                ++iter;
            }
        }
        bounds.push_back(
            cal_bounds{ vec2(bc.bounds.x, bc.bounds.z), ent });
    }
    // check against y axis
    for(auto&& [lhs, rhs] : overlaps)
    {
        auto& bclhs = registry.get<box_collider>(lhs);
        auto& bcrhs = registry.get<box_collider>(rhs);
        if(bcrhs.bounds.w >= bclhs.bounds.y
           && bclhs.bounds.w >= bcrhs.bounds.y)
        {
            // COLLISION DETECTED
        }
    }
#endif // collision
}
void physics_system::move_entity(entity ent, const vec2& to)
{
    auto& trns  = ent.get_component<transform>();
    auto& rb    = ent.get_component<rigidbody>();
    rb.velocity = (to - trns.position) / game::get_game_clock()->delta_time();
    ;
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
    rb.force += amount;
}
void physics_system::add_torque(entity ent, f32 amount)
{
    auto& rb = ent.get_component<rigidbody>();
    rb.torque += amount;
}
} // namespace lemon
