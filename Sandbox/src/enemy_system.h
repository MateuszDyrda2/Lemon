#pragma once

#include <assets/asset.h>
#include <events/events.h>
#include <world/service_container.h>
#include <world/system.h>
#include <world/entity.h>

using namespace lemon;
class enemy_system : public system
{
  public:
    SYSTEM(enemy_system, scene, event_queue, asset_storage);
    ~enemy_system();

    void on_update(event_args* e);
    void on_physics(event_args* e);

  private:
    scene& _scene;
    event_queue& _eventQueue;
    asset_storage& _assetStorage;

    event_queue::listener_handle update;
    event_queue::listener_handle physics;
    f32 timePassed;


  private:
    entity create_enemy(vec2 position);
    entity create_fireball(vec2 position, vec2 target);
};