/** @file window_events.h
 * @brief File declaring event types send by a window
 */
#pragma once

#include <core/lemon_types.h>
#include <events/events.h>

namespace lemon {
struct LEMON_API window_close : public event_args
{ };

struct LEMON_API window_size : public event_args
{
    int width, height;

    window_size(int width, int height):
        width(width), height(height) { }
};

struct LEMON_API framebuffer_size : public event_args
{
    int width, height;

    framebuffer_size(int width, int height):
        width(width), height(height) { }
};

struct LEMON_API window_content_scale : public event_args
{
    f32 xscale, yscale;

    window_content_scale(f32 xscale, f32 yscale):
        xscale(xscale), yscale(yscale) { }
};

struct LEMON_API window_pos : public event_args
{
    int xpos, ypos;

    window_pos(int xpos, int ypos):
        xpos(xpos), ypos(ypos) { }
};

struct LEMON_API window_iconify : public event_args
{
    int iconified;

    window_iconify(int iconified):
        iconified(iconified) { }
};

struct LEMON_API window_maximize : public event_args
{
    int maximized;

    window_maximize(int maximized):
        maximized(maximized) { }
};

struct LEMON_API window_focus : public event_args
{
    int focused;

    window_focus(int focused):
        focused(focused) { }
};

struct LEMON_API window_refresh : public event_args
{ };
}