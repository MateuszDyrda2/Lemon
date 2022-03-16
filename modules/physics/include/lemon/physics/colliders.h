#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>

#include <utility>

namespace lemon {
struct box_collider;
struct circle_collider;
struct collider_base
{
    virtual bool detect_collision(box_collider*) const noexcept               = 0;
    virtual bool detect_collision(circle_collider*) const noexcept            = 0;
    virtual std::pair<vec2, vec2> get_extends(const vec2& pos) const noexcept = 0;

    vec2 offset;
};
struct box_collider : public collider_base
{
    bool detect_collision(box_collider*) const noexcept;
    bool detect_collision(circle_collider*) const noexcept;
    std::pair<vec2, vec2> get_extends(const vec2& pos) const noexcept;

    vec2 size;
};
struct circle_collider : public collider_base
{
    bool detect_collision(box_collider*) const noexcept override;
    bool detect_collision(circle_collider*) const noexcept override;
    std::pair<vec2, vec2> get_extends(const vec2& pos) const noexcept override;

    f32 radius;
};
} // namespace lemon
