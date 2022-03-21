#pragma once

#include "key_codes.h"
#include <lemon/events/event.h>

namespace lemon {
struct KeyPressed : public event_args
{
    KeyPressed(key::keycode keycode,
               int scancode,
               key::action action,
               key::keymod keymod):
        keycode(keycode),
        scancode(scancode),
        action(action), keymod(keymod) { }

    key::keycode keycode;
    int scancode;
    key::action action;
    key::keymod keymod;
};
struct MouseButtonPressed : public event_args
{
    MouseButtonPressed(key::mouse button,
                       key::action action,
                       key::keymod keymod):
        button(button),
        action(action), keymod(keymod) { }

    key::mouse button;
    key::action action;
    key::keymod keymod;
};
struct MouseScroll : public event_args
{
    MouseScroll(double xoffset, double yoffset):
        xoffset(xoffset), yoffset(yoffset) { }
    double xoffset, yoffset;
};
struct WindowClose : public event_args
{
};
struct WindowSize : public event_args
{
    WindowSize(int width, int height):
        width(width), height(height) { }
    int width, height;
};
struct FramebufferSize : public event_args
{
    FramebufferSize(int width, int height):
        width(width), height(height) { }
    int width, height;
};
struct WindowContentScale : public event_args
{
    WindowContentScale(float xscale, float yscale):
        xscale(xscale), yscale(yscale) { }
    float xscale, yscale;
};
struct WindowPos : public event_args
{
    WindowPos(int xpos, int ypos):
        xpos(xpos), ypos(ypos) { }
    int xpos, ypos;
};
struct WindowIconify : public event_args
{
    WindowIconify(int iconified):
        iconified(iconified) { }
    int iconified;
};
struct WindowMaximize : public event_args
{
    WindowMaximize(int maximized):
        maximized(maximized) { }
    int maximized;
};
struct WindowFocused : public event_args
{
    WindowFocused(int focused):
        focused(focused) { }
    int focused;
};
struct WindowRefresh : public event_args
{
};

} // namespace lemon
