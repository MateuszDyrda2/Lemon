#pragma once

#include "../texture.h"

#include <assets/asset.h>
#include <core/math/color.h>
#include <core/math/vec4.h>
#include <world/reflection.h>

namespace lemon {
struct LEMON_API
    [[lemon::component]] sprite_renderer
{
    [[lemon::field]] color col{ 1.0f, 1.0f, 1.0f, 1.0f };
    [[lemon::field]] vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    [[lemon::field]] asset<texture> tex{};
};
}
