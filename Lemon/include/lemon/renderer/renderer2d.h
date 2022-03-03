#pragma once

#include <lemon/game/basic_components.h>
#include <lemon/math/mat4.h>

namespace lemon {
/** Base interface for 2d renderers */
class LEMON_PUBLIC renderer2d
{
  public:
    renderer2d() = default;
    virtual ~renderer2d() = default;
    /** @brief Prepares for rendering in this frame
     * @param viewProj view-project matrix of the main camera
     */
    virtual void start_render(const mat4& viewProj) = 0;
    /** @brief Renders a sprite
     * @param sComponent sprite renderer component of the entity
     * @param tComponent transform component of the entity
     */
    virtual void render_sprite(sprite_renderer& sComponent, transform& tComponent) = 0;
    /** @brief Finishes the rendering in this frame
     */
    virtual void end_render() = 0;
};
} // namespace lemon
