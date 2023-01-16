/** @file sat_algorithm.h
 * @brief Separating Axis Theorem algorihtm implementation
 */
#pragma once

#include <core/defines.h>
#include <core/lemon_types.h>
#include <optional>
#include <utility>

#include "shapes.h"

namespace lemon {
/** Class implementing the Separating Axis Theorem algorihtm
 * for checking for overlaps
 * https://en.wikipedia.org/wiki/Hyperplane_separation_theorem
 */
class LEMON_API sat_algorithm
{
  public:
    /** @brief Check for overlap of two box shapes
     * @param lhs first shape
     * @param rhs second shape
     * @return optional value with axis and distance if overlapped
     */
    std::optional<mtv> operator()(
        const box_shape& lhs,
        const box_shape& rhs) const noexcept;
    /** @brief Check for overlap of box with a circle
     * @param lhs first shape
     * @param rhs second shape
     * @return optional value with axis and distance if overlapped
     */
    std::optional<mtv> operator()(
        const box_shape& lhs,
        const circle_shape& rhs) const noexcept;
    /** @brief Check for overlap of two circle shapes
     * @param lhs first shape
     * @param rhs second shape
     * @return optional value with axis and distance if overlapped
     */
    std::optional<mtv> operator()(
        const circle_shape& lhs,
        const circle_shape& rhs) const noexcept;

  private:
    std::pair<f32, f32>
    project_box(const box_shape& shape, const vec2& axis)
        const noexcept;
    std::pair<f32, f32>
    project_circle(const circle_shape& shape, const vec2& axis)
        const noexcept;
};
}
