/** @file input.h
 * input manager
 */
#pragma once

#include "keycodes.h"
#include "window.h"

#include <lemon/core/hash_string.h>
#include <lemon/events/events.h>

#include <functional>
#include <memory>

namespace lemon {
struct input_event : public event_args
{ };
struct axis_event : public event_args
{
    axis_event(f32 value):
        value(value) { }
    f32 value;
};
/** Class responsible for processing input for the engine */
class input
{
  public:
    class impl;

  public:
    /** @brief Create an input instance
     * @param wnd main applicatio window
     */
    input(window& wnd, event_queue& queue);
    ~input();
    /** @brief Process input events for the frame */
    void update();
    /** @brief Bind a keyboard key with action to event
     * @param key keycode
     * @param action key action when the event should be fired
     * @param actionId nameid of the event
     */
    void add_key(keycode key, key_action action, hash_str actionId);
    /** @brief Bind a mouse key with action to event
     * @param button keycode
     * @param action key action when the event should be fired
     * @param actionId nameid of the event
     */
    void add_key(mouse button, key_action action, hash_str actionId);
    /** @brief Bind a gamepad button with action to event
     * @param button keycode
     * @param action key action when the event should be fired
     * @param actionId nameid of the event
     */
    void add_key(gamepad button, key_action action, hash_str actionId);
    /** @brief Bind a mouse axis to event
     * @param mouse mouse axis (either x or y)
     * @param actionId nameid of the event
     */
    void add_axis(mouse_axis mouse, hash_str actionId);
    /** @brief Bind a gamepad axis to event
     * @param gamepad gamepad axis
     * @param actionId nameid of the event
     */
    void add_axis(gamepad_axis gamepad, hash_str actionId);
    /** @brief Bind a keyboard axis to event
     * @param axis a pair of keycodes that represent an axis
     * @param actionId nameid of the event
     */
    void add_axis(std::pair<keycode, keycode> axis, hash_str actionId);

  private:
    std::unique_ptr<impl> pImpl;
};
} // namespace lemon
