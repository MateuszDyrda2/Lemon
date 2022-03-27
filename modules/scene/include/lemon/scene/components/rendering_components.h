#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/scene/reflection.h>

#include <lemon/assets/asset.h>
#include <lemon/core/math/color.h>
#include <lemon/core/math/vec4.h>
#include <lemon/rendering/texture.h>

namespace lemon {
/* Component for rendering 2D sprites */
struct LEMON_PUBLIC sprite_renderer : public component
{
    color col{ 1.0f, 1.0f, 1.0f, 1.0f };
    vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    asset<texture> text{};
    sprite_renderer() = default;
    sprite_renderer(const asset<texture>& tex):
        text(tex) { }
    sprite_renderer(const color& col,
                    const vec4& texCoords,
                    const asset<texture>& tex):
        col(col),
        texCoords(texCoords), text(tex) { }
    ~sprite_renderer() = default;

    LEMON_REFLECT(sprite_renderer, col, texCoords, text);
};
} // namespace lemon
