#include <physics/shapes.h>

#include <core/math/math.h>

namespace lemon {
box_shape create_box(vec2 hSize, vec2 center, f32 degrees)
{
    const auto rads = radians(degrees);
    const auto s    = sin(rads);
    const auto c    = cos(rads);

    box_shape shape = {
        .vertices = {
            { -hSize.x, -hSize.y },
            { hSize.x, -hSize.y },
            { hSize.x, hSize.y },
            { -hSize.x, hSize.y } },
    };

    std::transform(
        shape.vertices, shape.vertices + 4,
        shape.vertices,
        [&](const auto& val) {
            return vec2{ center.x + (c * val.x - s * val.y),
                         center.y + (s * val.x + c * val.y) };
        });
    return shape;
}
circle_shape create_circle(f32 radius, vec2 center)
{
    return circle_shape{ .center = center, .radius = radius };
}
}
