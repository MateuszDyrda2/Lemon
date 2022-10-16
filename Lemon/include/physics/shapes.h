#pragma once

#include <core/lemon_types.h>
#include <core/math/vec2.h>

namespace lemon {
struct mtv
{
    f32 overlap;
    vec2 axis;
};

struct box_shape
{
    vec2 vertices[4];
};

struct circle_shape
{
    vec2 center;
    f32 radius;
};

box_shape create_box(vec2 hSize, vec2 center, f32 degrees);
circle_shape create_circle(f32 radius, vec2 center);
}
