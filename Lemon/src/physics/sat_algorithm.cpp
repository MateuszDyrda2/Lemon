#include <physics/sat_algorithm.h>

#include <core/math/math.h>

#include <limits>
#include <set>

namespace lemon {

std::optional<mtv> sat_algorithm::operator()(
    const box_shape& lhs,
    const box_shape& rhs) const noexcept
{
    vec2 axis[] = {
        normalize(lhs.vertices[1] - lhs.vertices[0]),
        normalize(lhs.vertices[2] - lhs.vertices[1]),
        normalize(rhs.vertices[1] - rhs.vertices[0]),
        normalize(rhs.vertices[2] - rhs.vertices[1]),
    };

    int axisCount = (axis[0] == axis[2] || axis[0] == axis[3]) ? 2 : 4;
    mtv t         = { .overlap = std::numeric_limits<f32>::max(), .axis = {} };

    for (int i = 0; i < axisCount; ++i)
    {
        auto&& [lhsMin, lhsMax] = project_box(lhs, axis[i]);
        auto&& [rhsMin, rhsMax] = project_box(rhs, axis[i]);

        auto distance = std::min(lhsMax, rhsMax) - std::max(lhsMin, rhsMin);
        if (distance <= 0.f) return {};
        if (distance >= t.overlap) continue;

        t = { .overlap = distance, .axis = axis[i] };
    }
    return t;
}

std::optional<mtv> sat_algorithm::operator()(
    const box_shape& lhs,
    const circle_shape& rhs) const noexcept
{
    vec2 axis[] = {
        normalize(lhs.vertices[1] - lhs.vertices[0]),
        normalize(lhs.vertices[2] - lhs.vertices[1]),
    };

    mtv t = { .overlap = std::numeric_limits<f32>::max(), .axis = {} };
    for (const auto& a : axis)
    {
        const auto&& [lhsMin, lhsMax] = project_box(lhs, a);
        const auto&& [rhsMin, rhsMax] = project_circle(rhs, a);
        auto distance                 = std::min(lhsMax, rhsMax) - std::max(lhsMin, rhsMin);

        if (distance <= 0.f) return {};
        if (distance >= t.overlap) continue;

        t = { .overlap = distance, .axis = a };
    }
    return t;
}

std::optional<mtv> sat_algorithm::operator()(
    const circle_shape& lhs,
    const circle_shape& rhs) const noexcept
{
    vec2 axis               = normalize(rhs.center - lhs.center);
    auto&& [lhsMin, lhsMax] = project_circle(lhs, axis);
    auto&& [rhsMin, rhsMax] = project_circle(rhs, axis);

    auto distance = std::min(lhsMax, rhsMax) - std::max(lhsMin, rhsMin);
    if (distance <= 0.f) return {};

    return mtv{ .overlap = distance, .axis = axis };
}

std::pair<f32, f32>
sat_algorithm::project_box(const box_shape& shape, const vec2& axis)
    const noexcept
{
    f32 _min, _max;
    _max = _min = dot(shape.vertices[0], axis);

    for (std::size_t i = 1; i < 4; ++i)
    {
        f32 p = dot(shape.vertices[i], axis);
        _min  = std::min(_min, p);
        _max  = std::max(_max, p);
    }
    return { _min, _max };
}

std::pair<f32, f32>
sat_algorithm::project_circle(const circle_shape& shape, const vec2& axis)
    const noexcept
{
    auto length         = shape.radius * axis;
    auto right          = dot(shape.center + length, axis);
    auto left           = dot(shape.center - length, axis);
    auto&& [_min, _max] = std::minmax(left, right);
    return { _min, _max };
}
}
