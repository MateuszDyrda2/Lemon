#include <lemon/physics/SAT.h>

#include <lemon/core/math/math.h>

#include <algorithm>
#include <numeric>

namespace lemon {
using namespace std;
std::optional<MTV> SAT::operator()(
    const box_shape& lhs,
    const box_shape& rhs) const noexcept
{
    // to get the normal: v(-x, y)
    vec2 axis[4] = {
        normalize(
            vec2(lhs.vertices[0].x - lhs.vertices[1].x,
                 lhs.vertices[1].y - lhs.vertices[0].y)),
        normalize(
            vec2(lhs.vertices[1].x - lhs.vertices[2].x,
                 lhs.vertices[2].y - lhs.vertices[1].y)),
        normalize(
            vec2(rhs.vertices[0].x - rhs.vertices[1].x,
                 rhs.vertices[1].y - rhs.vertices[0].y)),
        normalize(
            vec2(rhs.vertices[1].x - rhs.vertices[2].x,
                 rhs.vertices[2].y - rhs.vertices[1].y))
    };

    MTV mtv{ std::numeric_limits<f32>::max(), {} };
    for(const auto& a : axis)
    {
        auto&& [lhsMin, lhsMax] = project_box(lhs, a);
        auto&& [rhsMin, rhsMax] = project_box(lhs, a);

        if(
            auto dist = min(lhsMax, rhsMax) - max(lhsMin, rhsMin);
            dist > 0.f)
        {
            if(dist < mtv.overlap)
            {
                mtv.overlap = dist;
                mtv.axis    = a;
            }
        }
        else
            return {};
    }
    return mtv;
}
std::optional<MTV>
SAT::operator()(const circle_shape& lhs, const circle_shape& rhs) const noexcept
{
    vec2 axis               = normalize(rhs.center - lhs.center);
    auto&& [lhsMin, lhsMax] = project_circle(lhs, axis);
    auto&& [rhsMin, rhsMax] = project_circle(rhs, axis);
    if(
        auto dist = min(lhsMax, rhsMax) - max(lhsMin, rhsMin);
        dist > 0.f)
    {
        return MTV{ dist, axis };
    }
    else
        return {};
}
std::pair<f32, f32>
SAT::project_box(const box_shape& shape, const vec2& axis) const noexcept
{
    f32 _min = numeric_limits<f32>::max();
    f32 _max = numeric_limits<f32>::min();
    std::for_each(
        shape.vertices, shape.vertices + 4,
        [&](const vec2& vertex) {
            f32 p = dot(vertex, axis);
            _min  = min(_min, p);
            _max  = max(_max, p);
        });
    return { _min, _max };
}
std::pair<f32, f32>
SAT::project_circle(const circle_shape& shape, const vec2& axis) const noexcept
{
    auto length       = shape.radius * axis;
    auto right        = dot(shape.center + length, axis);
    auto left         = dot(shape.center - length, axis);
    auto [_min, _max] = minmax(left, right);
    return { _min, _max };
}
} // namespace lemon
