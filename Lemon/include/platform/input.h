/** @file input.h
 * input manager
 */
#pragma once

#include "keycodes.h"
#include "window.h"

#include <core/hash_string.h>
#include <events/events.h>

#include <functional>
#include <memory>

namespace lemon {
struct LEMON_API input_event : public event_args
{ };
struct LEMON_API axis_event : public event_args
{
    axis_event(f32 value):
        value(value) { }
    f32 value;
};
struct LEMON_API pair_keycodes
{
    keycode first, second;
};
/** Class responsible for processing input for the engine */
class LEMON_API input
{
  public:
    class impl;

  public:
    /** @brief Create an input instance
     * @param wnd main application window
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
    void add_key(keycode key, key_action action, hashstr actionId);
    /** @brief Bind a mouse key with action to event
     * @param button keycode
     * @param action key action when the event should be fired
     * @param actionId nameid of the event
     */
    void add_key(mouse button, key_action action, hashstr actionId);
    /** @brief Bind a gamepad button with action to event
     * @param button keycode
     * @param action key action when the event should be fired
     * @param actionId nameid of the event
     */
    void add_key(gamepad button, key_action action, hashstr actionId);
    /** @brief Bind a mouse axis to event
     * @param mouse mouse axis (either x or y)
     * @param actionId nameid of the event
     */
    void add_axis(mouse_axis mouse, hashstr actionId);
    /** @brief Bind a gamepad axis to event
     * @param gamepad gamepad axis
     * @param actionId nameid of the event
     */
    void add_axis(gamepad_axis gamepad, hashstr actionId);
    /** @brief Bind a keyboard axis to event
     * @param axis a pair of keycodes that represent an axis
     * @param actionId nameid of the event
     */
    void add_axis(pair_keycodes axis, hashstr actionId);
    /** @brief Check if key's current state matches the argument
     * @param key keycode to check
     * @param action action to compare
     * @return true if the key is in the specified state */
    bool check_key(keycode key, key_action action);
    /** @brief Check if mouse button's current state matches the argument
     * @param key mouse button keycode to check
     * @param action action to compare
     * @return true if the button is in the specified state */
    bool check_key(mouse button, key_action action);
    /** @brief Check if gamepad button's current state matches the argument
     * @param key keycode to check
     * @param action action to compare
     * @return true if the button is in the specified state */
    bool check_key(gamepad button, key_action action);
    /** @brief Get value of axis
     * @param axis pair of keycodes
     * @return value in range[-1, 1] representing current axis state */
    f32 check_axis(pair_keycodes axis);
    /** @brief Get value of gamepad axis
     * @param axis gamepad axis
     * @return value in range[-1, 1] representing current axis state */
    f32 check_axis(gamepad_axis gamepad);
    /** @brief Get value of mouse axis
     * @param axis mouse axis (x,y)
     * @return current mouse position */
    f32 check_axis(mouse_axis mouse);

  private:
    std::unique_ptr<impl> pImpl;
};
} // namespace lemon
