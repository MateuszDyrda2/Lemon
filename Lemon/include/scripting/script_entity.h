#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>
#include <scripting/message_bus.h>

namespace lemon {

class scene;
struct script_animator
{
    void start_animation(const char* animation) const;
    void stop_animations() const;

    scene* _scene;
    u32 handle;
};

struct script_message_proxy
{
    script_message_proxy& arg(i32);
    script_message_proxy& arg(f32);
    script_message_proxy& arg(bool);
    script_message_proxy& arg(vec2);
    script_message_proxy& arg(const char*);
    void push();

    message_bus::message_builder builder;
    u32 handle;
};

struct LEMON_API script_entity
{
    u32 handle;
    const script_animator& get_animator() const { return animator; }

    script_message_proxy create_message(const char* name);

    message_bus& _messageBus;
    scene* _scene;
    script_animator animator;

    script_entity(message_bus& _messageBus, scene* _scene, u32 handle):
        handle(handle), _messageBus(_messageBus), _scene(_scene),
        animator{ ._scene = _scene, .handle = handle }
    { }
};
}
