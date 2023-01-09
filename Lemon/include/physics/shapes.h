/** @file shapes.h
 * @brief File defining basic shapes needed for collisions
 */
#pragma once

#include <core/lemon_types.h>
#include <core/math/vec2.h>

namespace lemon {
/** type representing a Minimum Translation Vector */
struct mtv
{
    f32 overlap; ///< Overlap size
    vec2 axis;   ///< Overlap axis
};

/** type representing a box */
struct box_shape
{
    vec2 vertices[4]; ///< Vertex positions
};

/** type representing a circle */
struct circle_shape
{
    vec2 center; ///< center coords
    f32 radius;  ///< circle radius
};

/** @brief Creates a box with a given half-size, center and rotation
 * @param hSize half-width, half-height
 * @param center box coords
 * @param degrees rotation
 * @return box shape
 */
box_shape create_box(vec2 hSize, vec2 center, f32 degrees);
/** @brief Creates a circle with given, radius and center
 * @param radius circle radius
 * @param center circle coords
 * @return circle shape
 */
circle_shape create_circle(f32 radius, vec2 center);
}
