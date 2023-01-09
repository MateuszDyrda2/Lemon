/** @file script_entity.h
 * @brief File with a script entity definition
 */
#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>
#include <scripting/message_bus.h>

namespace lemon {

class scene;
/** struct for dealing with animations in scripts */
struct script_animator
{
    /** @brief Start animation
     * @param animation animation name
     */
    void start_animation(const char* animation) const;
    /** @brief Stop animation */
    void stop_animations() const;

    scene* _scene;
    u32 handle;
};
/** struct for script messages */
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
/** struct for representing entity in a lua script */
struct LEMON_API script_entity
{
    u32 handle;
    message_bus& _messageBus;
    scene* _scene;
    script_animator animator;

    /** @return get animation object */
    const script_animator& get_animator() const { return animator; }
    /** @brief Create a message */
    script_message_proxy create_message(const char* name);
    /** @brief Creates a script entity
     * @param _messageBus message bus reference
     * @param _scene scene pointer
     * @param handle entity handle
     */
    script_entity(message_bus& _messageBus, scene* _scene, u32 handle):
        handle(handle), _messageBus(_messageBus), _scene(_scene),
        animator{ ._scene = _scene, .handle = handle }
    { }
};
}
