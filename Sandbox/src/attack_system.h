#pragma once

#include <world/system.h>
#include <world/service_container.h>
#include <events/events.h>
#include <assets/asset.h>

using namespace lemon;
class attack_system : public system 
{
    public:
    SYSTEM(attack_system, scene, event_queue, asset_storage);
    ~attack_system();

    private:
    scene& _scene;
    event_queue& _eventQueue;
    asset_storage& _assetStorage;

    event_queue::listener_handle onFire;
};