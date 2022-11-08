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

    [[lemon::field]] entity_t first{ entt::null };
    [[lemon::field]] entity_t next{ entt::null };
    [[lemon::field]] entity_t parent{ entt::null };
    [[lemon::field]] std::size_t order{};

    transform() = default;
    transform(vec2 position):
        position(position) { }
    transform(vec2 position, vec2 scale, f32 rotation):
        position(position), scale(scale), rotation(rotation) { }
};

struct LEMON_API model
{
    mat4 matrix{ 1.0f };
};

struct LEMON_API [[lemon::component]] main_camera_t
{ };

struct LEMON_API [[lemon::component]] camera
{
    [[lemon::field]] vec4 viewport{ 0.0f, 0.0f, 1.0f, 1.0f };
    [[lemon::field]] f32 size{ 1.0f };
};

LEMON_TAG(main_camera_t);
LEMON_REFL(transform, position, scale, rotation, first, next, parent, order);
LEMON_REFL(model, matrix);
LEMON_REFL(camera, viewport, size);
}
