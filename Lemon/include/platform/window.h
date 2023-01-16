/** @file window.h
 * @brief File containing window definition
 */
#pragma once

#include "window_events.h"

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/vec2.h>

#include <string>

namespace lemon {
/** Class representation of a game window */
class LEMON_API window
{
  public:
    using native_handle_t = void*;

  public:
    /** @brief Creates a window
     * @param name window name
     * @param size window size
     */
    window(const std::string& name, const ivec2& size);
    ~window();

    /** @return window size */
    auto get_size() const { return size; }
    /** @return window handle */
    auto get_handle() const { return handle; }
    /** @return window name */
    const auto& get_name() const { return name; }
    /** @brief Updates the window
     * @return should the window close?
     */
    bool update();

  private:
    std::string name;
    native_handle_t handle;
    ivec2 size;
};
} // namespace lemon
