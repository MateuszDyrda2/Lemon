#pragma once

#include "cpp_script.h"
#include "entity.h"

#include <lemon/assets/asset.h>
#include <lemon/audio/sound.h>
#include <lemon/audio/sound_source.h>
#include <lemon/core/basic_types.h>
#include <lemon/renderer/texture.h>

#include <lemon/math/mat4.h>
#include <lemon/math/vec3.h>
#include <lemon/math/vec4.h>

namespace lemon {
struct tag
{
    string_id id;
};
struct dirty
{ };
struct transform
{
    vec3 position{ 0.f, 0.f, 0.f };
    vec3 scale{ 1.f, 1.f, 1.f };
    f32 rotation{ 0.0f };
    mat4 model{ 1.f };
    entity_handle first{ entt::null };
    entity_handle next{ entt::null };
    entity_handle parent{ entt::null };
    size_type order{};

    transform() = default;
    transform(const vec3 position, const vec3 scale, f32 rotation,
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
    vec4 viewport{};
    mat4 projection{ 1.0f };
};
struct sprite_renderer
{
    vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    asset<texture> text{};
    sprite_renderer() = default;
    sprite_renderer(const asset<texture>& tex):
        text(tex) { }
};
struct audio_source
{
    sound_source source;
    asset<sound> sound{};
    f32 pitch{ 1.0f };
    f32 gain{ 1.0f };
    bool loop{ false };
};
struct audio_listener
{
    f32 masterGain{ 1.0f };
};
struct play
{ };
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
