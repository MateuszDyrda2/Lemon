#pragma once

#include <lemon/game/basic_components.h>
#include <lemon/math/mat4.h>

namespace lemon {
class LEMON_PUBLIC renderer2d
{
  public:
    virtual void start_render(const mat4& viewProj)                                = 0;
    virtual void render_sprite(sprite_renderer& sComponent, transform& tComponent) = 0;
    virtual void end_render()                                                      = 0;
};
} // namespace lemon
