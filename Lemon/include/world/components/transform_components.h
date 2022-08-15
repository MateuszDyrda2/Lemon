#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

#include "../scene.h"

namespace lemon {
struct LEMON_API dirty_t
{ };

struct LEMON_API transform
{
    vec2 position{ 0.0f, 0.0f };
    vec2 scale{ 1.0f, 1.0f };
    f32 rotation{ 0.0f };

    entity first{ entt::null };
    entity next{ entt::null };
    entity parent{ entt::null };
    std::size_t order{};
};

struct LEMON_API model
{
    mat4 matrix{ 1.0f };
};

struct LEMON_API main_camera_t
{ };

struct LEMON_API camera
{
    vec4 viewport{ 0.0f, 0.0f, 1.0f, 1.0f };
    f32 size{ 10.0f };
};
}
