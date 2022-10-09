#include "movement_system.h"
#include "player_components.h"
#include "world/components/transform_components.h"
#include "world/systems/transform_system.h"

using namespace lemon;
movement_system::movement_system(scene& _scene,
                                 event_queue& _eventQueue)
{
    horizontal = _eventQueue["Horizontal"_hs] += [&](event_args* e) {
        auto value  = static_cast<axis_event*>(e)->value;
        auto& reg   = _scene.get_registry();
        auto player = _scene.get_entity(reg.view<player_t>().front());
        transform_system::move_by(player, { value, 0 });
    };
    vertical = _eventQueue["Vertical"_hs] += [&](event_args* e) {
        auto value  = static_cast<axis_event*>(e)->value;
        auto& reg   = _scene.get_registry();
        auto player = _scene.get_entity(reg.view<player_t>().front());
        transform_system::move_by(player, { 0, value });
    };
}

movement_system::~movement_system()
{
}
