#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>
#include <lemon/core/string_id.h>

#include <lemon/scene/entity.h>
#include <lemon/scene/reflection.h>

namespace lemon {
/** Attached to the entity with transform component
 * whose model needs to be recalculated */
struct LEMON_PUBLIC dirty
{ };
/** Attached when entity is enabled */
struct LEMON_PUBLIC enabled
{ };
/** Id of the entity the component is attached to */
struct LEMON_PUBLIC tag : public component
{

    string_id id;
    tag() = default;
    tag(string_id id):
        id(id) { }
    ~tag() = default;

    LEMON_REFLECT(tag, id);
};

/** Transform component containing the position, rotation and
 * scale of the entity in the game world and also relationships */
struct LEMON_PUBLIC transform : public component
{

    vec2 position{ 0.f, 0.f };
    vec2 scale{ 1.f, 1.f };
    f32 rotation{ 0.0f };
    mat4 model{ 1.f };
    entity_handle first{ entt::null };
    entity_handle next{ entt::null };
    entity_handle parent{ entt::null };
    size_type order{};

    transform() = default;
    transform(const vec2 position, const vec2 scale, f32 rotation,
              entity_handle parent, size_type order):
        position(position),
        scale(scale),
        rotation(rotation), parent(parent), order(order)
    { }
    transform(entity_handle parent):
        parent(parent) { }
    ~transform() = default;

    LEMON_REFLECT(transform, position, scale, rotation,
                  first, next, parent, order);
};
/** Camera component */
struct LEMON_PUBLIC camera : public component
{
    vec4 viewport{};
    mat4 projection{ 1.0f };
    camera() = default;
    camera(vec4 vp):
        viewport(vp){};
    camera(vec4 vp, const mat4& proj):
        viewport(vp), projection(proj) { }
    ~camera() = default;

    LEMON_REFLECT(camera, viewport);
};

} // namespace lemon
