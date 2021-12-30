#pragma once

#include "cpp_script.h"
#include "entity.h"

#include <river/core/basic_types.h>
#include <river/renderer/texture.h>
#include <river/resources/resource.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace river {
struct dirty
{ };
struct transform
{
    glm::vec3 position{ 0.f, 0.f, 0.f };
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    f32 rotation{ 0.0f };
    glm::mat4 model{ 1.f };
    size_type order{};
    ptr<transform> first{};
    ptr<transform> next{};
    ptr<transform> parent{};
    transform(const glm::vec3 position, const glm::vec3 scale, f32 rotation,
              ptr<transform> parent):
        position(position),
        scale(scale),
        rotation(rotation), parent(parent), order(parent->order + 1)
    {
    }
    transform(ptr<transform> parent):
        parent(parent) { }
};
struct camera
{
    glm::vec4 viewport;
    glm::mat4 projection;
};
struct sprite_renderer
{
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    resource<texture> texture;
};
struct script_component
{
    owned<cpp_script> script{};

    script_component(owned<cpp_script>&& script):
        script(std::move(script)) { }

    inline void on_create() { script->on_create(); }
    inline void on_update(float dt) { script->on_update(dt); }
    inline void on_late_update(float dt) { script->on_late_update(dt); }
    inline void on_destroy(float dt) { script->on_destroy(); }
};
template<>
struct entt::component_traits<river::transform> : basic_component_traits
{
    using in_place_delete = std::true_type;
};
} // namespace river
