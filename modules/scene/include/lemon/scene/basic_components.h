#pragma once

#include "cpp_script.h"
#include "entity.h"

#include <lemon/assets/asset.h>
#include <lemon/audio/sound.h>
#include <lemon/audio/sound_source.h>
#include <lemon/core/basic_types.h>
#include <lemon/rendering/texture.h>

#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>

namespace lemon {
/** Id of the entity the component is attached to */
struct LEMON_PUBLIC tag
{
    string_id id;
};
/** Attached to the entity with transform component
 * whose model needs to be recalculated */
struct LEMON_PUBLIC dirty
{ };
/** Attached when entity is enabled */
struct LEMON_PUBLIC enabled
{ };
/* Play tag attached to the entity with audio_source
 * that wants to play a sound in a specific frame */
struct LEMON_PUBLIC play
{ };
/** Transform component containing the position, rotation and
 * scale of the entity in the game world and also relationships */
struct LEMON_PUBLIC transform
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
    {
    }
    transform(entity_handle parent):
        parent(parent) { }
};
/** Camera component */
struct LEMON_PUBLIC camera
{
    vec4 viewport{};
    mat4 projection{ 1.0f };
};
/* Component for rendering 2D sprites */
struct LEMON_PUBLIC sprite_renderer
{
    vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    vec4 texCoords{ 0.0f, 0.0f, 1.0f, 1.0f };
    asset<texture> text{};
    sprite_renderer() = default;
    sprite_renderer(const asset<texture>& tex):
        text(tex) { }
};
/* Sound source in the game world */
struct LEMON_PUBLIC audio_source
{
    sound_source source;
    asset<sound> clip{};
    f32 pitch{ 1.0f };
    f32 gain{ 1.0f };
    bool loop{ false };
    audio_source() = default;
    audio_source(const asset<sound>& s):
        clip(s) { }
};
/* Sound listener in the game world (One must be available in the scene) */
struct LEMON_PUBLIC audio_listener
{
    f32 masterGain{ 1.0f };
};

} // namespace lemon
