#pragma once

#include "key_codes.h"
#include <lemon/events/event_bus.h>

namespace lemon {
struct KeyPressed : public event
{
    KeyPressed(key::keycode keycode,
               int scancode,
               key::action action,
               key::keymod keymod):
        event(string_id("KeyPressed")),
        keycode(keycode),
        scancode(scancode),
        action(action), keymod(keymod) { }

    key::keycode keycode;
    int scancode;
    key::action action;
    key::keymod keymod;
};
struct MouseButtonPressed : public event
{
    MouseButtonPressed(key::mouse button,
                       key::action action,
                       key::keymod keymod):
        event(string_id("MouseButtonPressed")),
        button(button),
        action(action), keymod(keymod) { }

    key::mouse button;
    key::action action;
    key::keymod keymod;
};
struct MouseScroll : public event
{
    MouseScroll(double xoffset, double yoffset):
        event(string_id("MouseScroll")),
        xoffset(xoffset), yoffset(yoffset) { }
    double xoffset, yoffset;
};
struct WindowClose : public event
{
    WindowClose():
        event(string_id("WindowClose")) { }
};
struct WindowSize : public event
{
    WindowSize(int width, int height):
        event(string_id("WindowSize")),
        width(width), height(height) { }
    int width, height;
};
struct FramebufferSize : public event
{
    FramebufferSize(int width, int height):
        event(string_id("FramebufferSize")),
        width(width), height(height) { }
    int width, height;
};
struct WindowContentScale : public event
{
    WindowContentScale(float xscale, float yscale):
        event(string_id("WindowContentScale")),
        xscale(xscale), yscale(yscale) { }
    float xscale, yscale;
};
struct WindowPos : public event
{
    WindowPos(int xpos, int ypos):
        event(string_id("WindowPos")),
        xpos(xpos), ypos(ypos) { }
    int xpos, ypos;
};
struct WindowIconify : public event
{
    WindowIconify(int iconified):
        event(string_id("WindowIconify")),
        iconified(iconified) { }
    int iconified;
};
struct WindowMaximize : public event
{
    WindowMaximize(int maximized):
        event(string_id("WindowMaximize")),
        maximized(maximized) { }
    int maximized;
};
struct WindowFocused : public event
{
    WindowFocused(int focused):
        event(string_id("WindowFocused")),
        focused(focused) { }
    int focused;
};
struct WindowRefresh : public event
{
    WindowRefresh():
        event(string_id("WindowRefresh")) { }
};

} // namespace lemon
