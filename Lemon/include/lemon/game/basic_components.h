#pragma once

#include "cpp_script.h"
#include "entity.h"

#include <lemon/core/basic_types.h>
#include <lemon/renderer/texture.h>
#include <lemon/resources/resource.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace lemon {
struct tag
{
    string_id id;
};
struct dirty
{ };
struct transform
{
    glm::vec3 position{ 0.f, 0.f, 0.f };
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    f32 rotation{ 0.0f };
    glm::mat4 model{ 1.f };
    entity_handle first{ entt::null };
    entity_handle next{ entt::null };
    entity_handle parent{ entt::null };
    size_type order{};

    transform() = default;
    transform(const glm::vec3 position, const glm::vec3 scale, f32 rotation,
              entity_handle parent, size_type order):
        position(position),
        scale(scale),
        rotation(rotation), parent(parent), order(order)
    {
    }
    transform(entity_handle parent):
        parent(parent) { }
};
struct camera
{
    glm::vec4 viewport{};
    glm::mat4 projection{ 1.0f };
};
struct sprite_renderer
{
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    resource<texture> text{};
    sprite_renderer(resource<texture> tex):
        text(tex) { }
};
struct script_component
{
    owned<cpp_script> script{};

    script_component(owned<cpp_script>&& script):
        script(std::move(script)) { }

    inline void on_create(entity_registry&, entity_handle) { script->on_create(); }
    inline void on_update(float dt) { script->on_update(dt); }
    inline void on_late_update(float dt) { script->on_late_update(dt); }
    inline void on_destroy(entity_registry&, entity_handle) { script->on_destroy(); }
};
} // namespace lemon
