#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <optional>

namespace lemon {
struct MTV
{
    f32 overlap;
    vec2 axis;
};
struct rotated_t
{ };
struct axis_aligned_t
{ };
struct box_shape
{
    vec2 vertices[4];
};
struct circle_shape
{
    vec2 center;
    f32 radius;
};
class SAT
{
  public:
    static constexpr axis_aligned_t axis_aligned{};
    static constexpr rotated_t rotated{};

  public:
    std::optional<MTV> operator()(
        const box_shape& lhs,
        const box_shape& rhs,
        axis_aligned_t) const noexcept;
    std::optional<MTV> operator()(
        const box_shape& lhs,
        const box_shape& rhs,
        rotated_t) const noexcept;
    std::optional<MTV> operator()(
        const circle_shape& lhs,
        const circle_shape& rhs) const noexcept;
    std::optional<MTV> operator()(
        const box_shape& lhs,
        const circle_shape& rhs) const noexcept;

  private:
    std::pair<f32, f32>
    project_box(const box_shape& shape, const vec2& axis) const noexcept;
    std::pair<f32, f32>
    project_circle(const circle_shape& shape, const vec2& axis) const noexcept;
};
} // namespace lemon
