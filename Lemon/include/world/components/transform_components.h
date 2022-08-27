#pragma once

#include "../reflection.h"
#include "../scene.h"

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

namespace lemon {
struct LEMON_API dirty_t
{ };

struct LEMON_API [[lemon::component]] transform
{
    [[lemon::field]] vec2 position{ 0.0f, 0.0f };
    [[lemon::field]] vec2 scale{ 1.0f, 1.0f };
    [[lemon::field]] f32 rotation{ 0.0f };

    [[lemon::field]] entity first{ entt::null };
    [[lemon::field]] entity next{ entt::null };
    [[lemon::field]] entity parent{ entt::null };
    [[lemon::field]] std::size_t order{};
};

struct LEMON_API [[lemon::component]] model
{
    [[lemon::field]] mat4 matrix{ 1.0f };
};

struct LEMON_API [[lemon::tag]] main_camera_t
{ };

struct LEMON_API [[lemon::component]] camera
{
    [[lemon::field]] vec4 viewport{ 0.0f, 0.0f, 1.0f, 1.0f };
    [[lemon::field]] f32 size{ 10.0f };
};
}
