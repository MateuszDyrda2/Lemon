/** @file rendering_components.h
 * @brief File containing rendering components declarations
 */
#pragma once

#include "../texture.h"
#include "rendering/animation_script.h"

#include <assets/asset.h>
#include <core/hash_string.h>
#include <core/math/color.h>
#include <core/math/vec4.h>

namespace lemon {
/** sprite render for entity with a texture */
struct LEMON_API [[lemon::component]] sprite_renderer
{
    [[lemon::field]] color col{ 1.0f, 1.0f, 1.0f, 1.0f };
    [[lemon::field]] vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    [[lemon::field]] asset<texture> tex{};
};

/** animation component */
struct LEMON_API [[lemon::component]] animation_component
{
    [[lemon::field]] ivec2 frameSize{};
    [[lemon::field]] asset<animation_script> animation;
};

/** message for specifying animation start */
struct LEMON_API start_animation_m
{
    hashstr name;
};

/** message for specifying animation stop */
struct LEMON_API stop_animation_m
{ };

/** tag for entities being animated */
struct LEMON_API animating_t
{
    f32 duration;
    hashstr animationName;

    animating_t(hashstr animationName):
        duration(0.0f), animationName(animationName) { }
};
}
