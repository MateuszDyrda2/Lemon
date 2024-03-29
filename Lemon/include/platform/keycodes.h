/** @file keycodes.h
 * @brief File with enum classes representing keys
 */
#pragma once

namespace lemon {
#define KEYBOARD_FIRST (keycode::space)
#define KEYBOARD_LAST  (keycode::menu)
#define KEYBOARD_COUNT (std::size_t(KEYBOARD_LAST) - std::size_t(KEYBOARD_FIRST) + 1)

#define MOUSE_FIRST (mouse::button_1)
#define MOUSE_LAST  (mouse::button_8)
#define MOUSE_COUNT (std::size_t(MOUSE_LAST) - std::size_t(MOUSE_FIRST) + 1)

#define MOUSE_AXIS_FIRST (mouse_axis::x)
#define MOUSE_AXIS_LAST  (mouse_axis::y)
#define MOUSE_AXIS_COUNT (std::size_t(MOUSE_AXIS_LAST) - std::size_t(MOUSE_AXIS_FIRST) + 1)

#define GAMEPAD_FIRST (gamepad::a)
#define GAMEPAD_LAST  (gamepad::dpad_left)
#define GAMEPAD_COUNT (std::size_t(GAMEPAD_LAST) - std::size_t(GAMEPAD_FIRST) + 1)

#define GAMEPAD_AXIS_FIRST (gamepad_axis::left_x)
#define GAMEPAD_AXIS_LAST  (gamepad_axis::right_trigger)
#define GAMEPAD_AXIS_COUNT (std::size_t(GAMEPAD_AXIS_LAST) - std::size_t(GAMEPAD_AXIS_FIRST) + 1)

/* printable keys */
enum key_action
{
    up       = 0,
    released = 1 << 0,
    pressed  = 1 << 1,
    down     = 1 << 2
};
enum class device
{
    keyboard,
    keyboard_axis,
    mouse,
    mouse_axis,
    gamepad,
    gamepad_axis
};
enum class keycode
{
    // Keyboard keys
    space         = 32,
    apostrophe    = 39 /* ' */,
    comma         = 44 /* , */,
    minus         = 45 /* - */,
    period        = 46 /* . */,
    slash         = 47 /* / */,
    key_0         = 48,
    key_1         = 49,
    key_2         = 50,
    key_3         = 51,
    key_4         = 52,
    key_5         = 53,
    key_6         = 54,
    key_7         = 55,
    key_8         = 56,
    key_9         = 57,
    semicolon     = 59,
    equal         = 61,
    a             = 65,
    b             = 66,
    c             = 67,
    d             = 68,
    e             = 69,
    f             = 70,
    g             = 71,
    h             = 72,
    i             = 73,
    j             = 74,
    k             = 75,
    l             = 76,
    m             = 77,
    n             = 78,
    o             = 79,
    p             = 80,
    q             = 81,
    r             = 82,
    s             = 83,
    t             = 84,
    u             = 85,
    v             = 86,
    w             = 87,
    x             = 88,
    y             = 89,
    z             = 90,
    left_bracket  = 91,
    backslash     = 92,
    right_bracket = 93,
    grave_accent  = 97,
    world_1       = 161 /* non-us #1 */,
    world_2       = 162 /* non-us #2 */,
    escape        = 256,
    enter         = 257,
    tab           = 258,
    backspace     = 259,
    insert        = 260,
    del           = 261,
    right_arrow   = 262,
    left_arrow    = 263,
    down_arrow    = 264,
    up_arrow      = 265,
    page_up       = 266,
    page_down     = 267,
    home          = 268,
    end           = 269,
    caps_lock     = 280,
    scroll_lock   = 281,
    num_lock      = 282,
    print_screen  = 283,
    pause         = 284,
    f1            = 290,
    f2            = 291,
    f3            = 292,
    f4            = 293,
    f5            = 294,
    f6            = 295,
    f7            = 296,
    f8            = 297,
    f9            = 298,
    f10           = 299,
    f11           = 300,
    f12           = 301,
    f13           = 302,
    f14           = 303,
    f15           = 304,
    f16           = 305,
    f17           = 306,
    f18           = 307,
    f19           = 308,
    f20           = 309,
    f21           = 310,
    f22           = 311,
    f23           = 312,
    f24           = 313,
    f25           = 314,
    kp_0          = 320,
    kp_1          = 321,
    kp_2          = 322,
    kp_3          = 323,
    kp_4          = 324,
    kp_5          = 325,
    kp_6          = 326,
    kp_7          = 327,
    kp_8          = 328,
    kp_9          = 329,
    kp_decimal    = 330,
    kp_divide     = 331,
    kp_multiply   = 332,
    kp_subtract   = 333,
    kp_add        = 334,
    kp_enter      = 335,
    kp_equal      = 336,
    left_shift    = 340,
    left_control  = 341,
    left_alt      = 342,
    left_super    = 343,
    right_shift   = 344,
    right_control = 345,
    right_alt     = 346,
    right_super   = 347,
    menu          = 348,
};
// Mouse buttons
enum class mouse
{
    button_1 = 349 + 0,
    button_2,
    button_3,
    button_4,
    button_5,
    button_6,
    button_7,
    button_8
};
// Mouse axis
enum class mouse_axis
{
    x = 357,
    y
};
enum class gamepad
{
    // Gamepad keys
    a = 360,
    b,
    x,
    y,
    left_bumper,
    right_bumper,
    back,
    start,
    guide,
    left_thumb,
    right_thumb,
    dpad_up,
    dpad_right,
    dpad_down,
    dpad_left
};
// Gamepad axis
enum class gamepad_axis
{
    left_x = 375,
    left_y,
    right_x,
    right_y,
    left_trigger,
    right_trigger
};
enum keymod
{
    SHIFT     = 0x0001,
    CONTROL   = 0x0002,
    ALT       = 0x0004,
    SUPER     = 0x0008,
    CAPS_LOCK = 0x0010,
    NUM_LOCK  = 0x0020
};
} // namespace lemon
