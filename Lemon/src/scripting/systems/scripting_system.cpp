#include "events/events.h"
#include "scripting/components/scripting_components.h"
#include "world/system.h"
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

    early = _eventQueue["EarlyUpdate"_hs] += [this](event_args* e) {
        this->on_early_update(e);
    };

    physics = _eventQueue["PhysicsUpdate"_hs] += [this](event_args* e) {
        this->on_physics(e);
    };

    mount = _eventQueue["OnSceneLoaded"_hs] += [this](event_args* e) {
        this->on_mount(e);
    };
}

scripting_system::~scripting_system()
{
}

void scripting_system::add_start(registry& _registry, entity_t handle)
{
    auto& _scriptComponent = _registry.get<script_component>(handle);
    _scriptComponent.scriptObject.get()->start(script_entity(_messageBus, &_scene, u32(handle)));
}

void scripting_system::on_mount([[maybe_unused]] event_args* e)
{
    for (auto&& [_entity, _scriptComponent] : _scene.view<script_component>().each())
    {
        auto scr = _scriptComponent.scriptObject.get();
        scr->start(script_entity(_messageBus, &_scene, u32(_entity)));
    }

    auto& _registry = _scene.get_registry();
    _registry.on_construct<script_component>().connect<&scripting_system::add_start>(this);
}

void scripting_system::on_update([[maybe_unused]] event_args* e)
{
    auto delta = get_event<update_event>(e).deltaTime;
    for (auto&& [_entity, _scriptComponent] : _scene.view<script_component>().each())
    {
        [[maybe_unused]] auto scr = _scriptComponent.scriptObject.get();
        scr->update(script_entity(_messageBus, &_scene, u32(_entity)), delta);
        if (auto messages = _messageBus.get_messages(underlying_type_t<entity_t>(_entity)))
        {
            for ([[maybe_unused]] auto&& message : *messages)
            {
                scr->execute(
                    script_entity(_messageBus, &_scene, u32(_entity)),
                    message.messageName, message.payload);
            }
        }
    }
    _messageBus.clear();
}

void scripting_system::on_early_update([[maybe_unused]] event_args* e)
{
    auto delta = get_event<update_event>(e).deltaTime;
    for (auto&& [_entity, _scriptComponent] : _scene.view<script_component>().each())
    {
        [[maybe_unused]] auto scr = _scriptComponent.scriptObject.get();
        scr->early(script_entity(_messageBus, &_scene, u32(_entity)), delta);
    }
    _messageBus.clear();
}

void scripting_system::on_physics([[maybe_unused]] event_args* e)
{
    auto delta = get_event<fixed_update_event>(e).fixedDelta;
    for (auto&& [_entity, _scriptComponent] : _scene.view<script_component>().each())
    {
        [[maybe_unused]] auto scr = _scriptComponent.scriptObject.get();
        scr->physics(script_entity(_messageBus, &_scene, u32(_entity)), delta);
    }
    _messageBus.clear();
}
}
