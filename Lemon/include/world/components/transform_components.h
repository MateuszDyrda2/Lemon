#pragma once

#include "../scene.h"

#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec4.h>

namespace lemon {
/** dirty tag */
struct LEMON_API dirty_t
{ };

/** entity transform */
struct LEMON_API [[lemon::component]] transform
{
    [[lemon::field]] vec2 position{ 0.0f, 0.0f };
    [[lemon::field]] vec2 scale{ 1.0f, 1.0f };
    [[lemon::field]] f32 rotation{ 0.0f };

    [[lemon::field]] entity_t first{ entt::null };
    [[lemon::field]] entity_t next{ entt::null };
    [[lemon::field]] entity_t parent{ entt::null };
    [[lemon::field]] std::size_t order{};

    [[lemon::field]] i32 layer{ 0 };

    transform() = default;
    transform(vec2 position):
        position(position) { }
    transform(vec2 position, vec2 scale, f32 rotation):
        position(position), scale(scale), rotation(rotation) { }
};

/** calculated model matrix */
struct LEMON_API model
{
    mat4 matrix{ 1.0f };
};

/** main camera tag */
struct LEMON_API [[lemon::component]] main_camera_t
{ };

/** camera with viewport and size */
struct LEMON_API [[lemon::component]] camera
{
    [[lemon::field]] vec4 viewport{ 0.0f, 0.0f, 1.0f, 1.0f };
    [[lemon::field]] f32 size{ 1.0f };
};
}
