#pragma once

#include "texture.h"

#include <assets/asset.h>
#include <core/lemon_types.h>
#include <core/math/color.h>
#include <core/math/mat4.h>
#include <core/math/vec3.h>

namespace lemon {
/** Base interface for 2d renderers */
class LEMON_API renderer2d
{
  public:
    renderer2d(asset_storage&) { }
    virtual ~renderer2d() = default;
    /** @brief Prepares for rendering in this frame
     * @param viewProj view-project matrix of the main camera
     */
    virtual void start_render(const mat4& viewProj) = 0;
    /** @brief Renders a sprite
     * @param sComponent sprite renderer component of the entity
     * @param tComponent transform component of the entity
     */
    virtual void render_sprite(const color& col, const vec4& texCoords, asset<texture>& tex,
                               const mat4& model) = 0;
    /** @brief Finishes the rendering in this frame
     */
    virtual void end_render() = 0;
};
} // namespace lemon
