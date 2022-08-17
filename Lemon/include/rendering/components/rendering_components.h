#pragma once

#include "../texture.h"

#include <assets/asset.h>
#include <core/math/color.h>
#include <core/math/vec4.h>
#include <world/reflection.h>

namespace lemon {
struct LEMON_API sprite_renderer
{
    color col{ 1.0f, 1.0f, 1.0f, 1.0f };
    vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    asset<texture> tex{};

    LEMON_REFL(sprite_renderer, col, texCoords, tex);
};
}
