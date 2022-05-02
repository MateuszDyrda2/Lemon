#include <lemon/physics/systems/collision_response_system.h>

#include <lemon/core/instrumentor.h>
#include <lemon/core/math/math.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scene/scene.h>

#include <numeric>

namespace lemon {
using namespace std;
collision_response_system::collision_response_system(ptr<scene> /*s*/, clock& clk, scheduler& sch):
    clk(clk), sch(sch)
{ }
collision_response_system::~collision_response_system()
{ }
static u64 hash_collision(u32 a, u32 b)
{
    auto [low, high] = minmax(a, b);
    return (u64(high) << 32) | low;
}
void collision_response_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    // TODO: send on collision
    // TODO: fix this somehow
    // 1. Use existing group to speedup lookup?
    // 2. cache results for pairs of rigidbodies?

    unordered_set<u64> newCollisions;
    unordered_set<u64> newTriggers;
    auto collisions = registry.view<collision_m>();
    for(auto&& [ent, coll] : collisions.each())
    {
        if(auto hColl = hash_collision(u32(coll.A), u32(coll.B));
           cachedCollisions.contains(hColl))
        {
            cachedCollisions.erase(hColl);
            newCollisions.insert(hColl);
            // on collision stay
        }
        else
        {
            newCollisions.insert(hColl);
            // on collsion enter
        }
    }
    for(auto&& inactive : cachedCollisions)
    {
        // on collision leave
    }
    cachedCollisions.swap(newCollisions);

    auto triggers = registry.view<trigger_m>();
    for(auto&& [ent, trg] : triggers.each())
    {
        if(auto hTrig = hash_collision(u32(trg.A), u32(trg.B));
           cachedTriggers.contains(hTrig))
        {
            cachedTriggers.erase(hTrig);
            newTriggers.insert(hTrig);
            // on trigger stay
        }
        else
        {
            newTriggers.insert(hTrig);
            // on trigger enter
        }
    }
    for(auto&& inactive : cachedTriggers)
    {
        // on trigger leave
    }
    cachedTriggers.swap(newTriggers);

    auto deltaTime = clk.delta_time();
    collisions.each(
        [&](auto& collision) {
            auto&& [trA, collA, rbA] = registry.get<transform, collider, rigidbody>(collision.A);
            auto&& [trB, collB]      = registry.get<transform, collider>(collision.B);

            // collision response
            auto rv = dot(rbA.velocity, collision.mtv.axis);
            if(rv <= 0)
            {
                auto bounciness = min(collA.bounciness, collB.bounciness);
                auto vj         = -(1 + bounciness) * rv;
                auto j          = vj;
                rbA.velocity += j * collision.mtv.axis;
                // trA.position += rbA.velocity * deltaTime;
            }
        });
    /*
        auto deltaTime = clk.delta_time();
        for(auto&& [ent, tr, rb] : registry.view<transform, rigidbody>().each())
        {
            tr.position += rb.velocity * deltaTime;
            registry.emplace_or_replace<dirty_t>(ent);
        }
    */
    /*
        auto collisions = registry.view<collision_m>();
        sch.for_each(
            collisions.begin(), collisions.end(),
            [&](auto ent) {
                auto&& [coll]            = collisions.get(ent);
                auto&& [trA, collA, rbA] = registry.get<transform, collider, rigidbody>(coll.A);
                auto&& [trB, collB]      = registry.get<transform, collider>(coll.B);

                // impulse collision response
                auto bounciness = std::min(collA.bounciness, collB.bounciness);
                auto vj         = -(1 + bounciness) * dot(rbA.velocity, coll.mtv.axis);
                auto j          = vj;
                rbA.velocity += j * coll.mtv.axis;
            });

        // TODO: trigger response
        auto triggers = registry.view<trigger_m>();
        sch.for_each(
            triggers.begin(), triggers.end(),
            [&](auto ent) {
                auto&& [trg] = triggers.get(ent);
                (void)trg;
            });
            */
}

} // namespace lemon
