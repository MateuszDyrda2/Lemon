#include "enemy_system.h"

#include "player_components.h"
#include <core/math/math.h>
#include <core/utils.h>
#include <physics/components/physics_components.h>
#include <rendering/components/rendering_components.h>
#include <world/components/entity_components.h>
#include <world/components/transform_components.h>
#include <world/scene.h>

using namespace lemon;
enemy_system::enemy_system(scene& _scene, event_queue& _eventQueue, asset_storage& _assetStorage):
    _scene(_scene), _eventQueue(_eventQueue), _assetStorage(_assetStorage), timePassed(0)
{
    update = _eventQueue["Update"_hs] += [this](event_args* e) {
        this->on_update(e);
    };

    physics = _eventQueue["Physics"_hs] += [this](event_args* e) {
        this->on_physics(e);
    };

    srand(time(0));
}

enemy_system::~enemy_system() { }

void enemy_system::on_update(event_args* e)
{
    static constexpr f32 period   = 2.f;
    static constexpr f32 lifetime = 1.5f;
    auto delta                    = get_event<update_event>(e).deltaTime;
    auto view                     = _scene.view<transform, enemy_component>();

    timePassed += delta;
    for (auto&& [_entity, _transform, _enemy] : view.each())
    {
        if ((_enemy.life += delta) > lifetime)
        {
            // fire
            auto player = _scene.view<player_t, transform>().front();
            auto ptr    = _scene.get<transform>(player);
            create_fireball(_transform.position, ptr.position);

            // die
            _scene.get_entity(_entity).emplace<destroy_m>();
        }
    }

    if (timePassed > period)
    {
        timePassed = 0;
        f32 x      = -900 + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (900 - (-900)));
        f32 y      = -600 + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (600 - (-600)));
        create_enemy({ x, y });
    }

    for (auto&& [_entity, _transform] : _scene.view<fireball_t, transform>().each())
    {
        if (!in_range(_transform.position.x, -900.f, 900.f)
            || !in_range(_transform.position.y, -600.f, 600.f))
        {
            _scene.get_entity(_entity).emplace<destroy_m>();
        }
    }
}

void enemy_system::on_physics(event_args*)
{
}

entity enemy_system::create_enemy(vec2 position)
{
    auto ent                   = _scene.create_entity("enemy"_hs, position);
    auto&& sr                  = ent.emplace<sprite_renderer>();
    sr.tex                     = _assetStorage.get_asset<texture>("dwarf_leader-m-001"_hs);
    ent.get<transform>().layer = 1;
    sr.texCoords               = { 0, 0, .33, .25 };
    ent.emplace<enemy_component>();
    return ent;
}

entity enemy_system::create_fireball(vec2 position, vec2 target)
{
    auto ent                   = _scene.create_entity("fireball"_hs, position);
    auto&& sr                  = ent.emplace<sprite_renderer>();
    sr.tex                     = _assetStorage.get_asset<texture>("fire"_hs);
    ent.get<transform>().layer = 1;
    auto&& rb                  = ent.emplace<rigidbody>();
    ent.emplace<fireball_t>();
    auto&& bc   = ent.emplace<box_collider>();
    rb.position = rb._oldPosition = position;
    rb.isKinetic                  = true;
    rb.mass                       = 0.1;
    rb.colliderType               = collider_type::box;
    bc.hSize                      = { 16, 16 };
    rb.velocity                   = 700.f * normalize(target - position);
    return ent;
}