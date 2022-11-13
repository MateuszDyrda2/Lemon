#include "scripting/components/scripting_components.h"
#include <scripting/systems/scripting_system.h>
#include <type_traits>
#include <world/scene.h>

namespace lemon {
using namespace std;
scripting_system::scripting_system(scene& _scene, event_queue& _eventQueue, message_bus& _messageBus):
    _scene(_scene), _messageBus(_messageBus)
{
    update = _eventQueue["Update"_hs] += [this](event_args* e) {
        this->on_update(e);
    };
}

scripting_system::~scripting_system()
{
}

void scripting_system::on_update([[maybe_unused]] event_args* e)
{
    for (auto&& [_entity, _scriptComponent] : _scene.view<script_component>().each())
    {
        if (auto messages = _messageBus.get_messages(underlying_type_t<entity_t>(_entity)))
        {
            auto scr = _scriptComponent.scriptObject.get();
            for (auto&& message : *messages)
            {
                scr->execute(
                    script_entity(_messageBus, &_scene, u32(_entity)),
                    message.messageName, message.payload);
            }
        }
    }
    _messageBus.clear();
}
}
