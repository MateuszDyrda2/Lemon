#include "attack_system.h"

#include <core/logger.h>
#include <world/scene.h>
#include <rendering/components/rendering_components.h>

using namespace lemon;
attack_system::attack_system(scene& _scene, event_queue& _eventQueue, asset_storage& _assetStorage):
    _scene(_scene), _eventQueue(_eventQueue), _assetStorage(_assetStorage)
{
    onFire = _eventQueue["fire"_hs] += [this](event_args*) {
        logger::info("fire");
    };
}

attack_system::~attack_system() { }