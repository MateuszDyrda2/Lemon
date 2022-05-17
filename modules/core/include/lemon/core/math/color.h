#pragma once

#include "vec3.h"
#include "vec4.h"

#include "../assert.h"
#include "../basic_types.h"
#include "../defines.h"

#include <cstddef>
#include <string>
#include <utility>

namespace lemon {
struct LEMON_PUBLIC color
{
    union
    {
        struct
        {
            f32 r, g, b, a;
        };
        vec4 rgba;
    };
    static constexpr color red() { return color(1.f, 0.f, 0.f, 1.f); }
    static constexpr color green() { return color(0.f, 1.f, 0.f, 1.f); }
    static constexpr color blue() { return color(0.f, 0.f, 1.f, 1.f); }
    static constexpr color white() { return color(1.f, 1.f, 1.f, 1.f); }
    static constexpr color black() { return color(0.f, 0.f, 0.f, 1.f); }

    constexpr color():
        rgba(0.f, 0.f, 0.f, 1.f) { }
    constexpr color(const vec3& rgb):
        rgba{ rgb.x, rgb.y, rgb.z, 1.f } { }
    constexpr color(const vec4& rgba):
        rgba(rgba) { }
    constexpr color(f32 red, f32 green, f32 blue):
        r(red), g(green), b(blue), a(1.f) { }
    constexpr color(f32 red, f32 green, f32 blue, f32 alpha):
        r(red), g(green), b(blue), a(alpha) { }
    constexpr color(const u8vec3& rgb):
        rgba{ normalize(rgb.x), normalize(rgb.y), normalize(rgb.z), 1.f } { }
    constexpr color(const u8vec4& rgba):
        rgba(normalize(rgba)) { }
    constexpr color(u8 red, u8 green, u8 blue):
        r(normalize(red)), g(normalize(green)),
        b(normalize(blue)), a(1.f) { }
    constexpr color(u8 red, u8 green, u8 blue, u8 alpha):
        r(normalize(red)), g(normalize(green)),
        b(normalize(blue)), a(normalize(alpha)) { }
    color(const char* hex)
    {
        int ir, ig, ib, ia;
        lemon_assert(hex[0] == '#');
        std::sscanf(hex, "%*c%02x%02x%02x%02x", &ir, &ig, &ib, &ia);
        rgba = vec4{ normalize(ir), normalize(ig), normalize(ib), normalize(ia) };
    }
    constexpr color(u32 hex):
        r{ normalize(u8(hex >> 24)) }, g{ normalize(u8(hex >> 16 & 0xFF)) },
        b{ normalize(u8(hex >> 8 & 0xFF)) }, a{ normalize(u8(hex & 0xFF)) } { }
    constexpr bool operator==(const color& other) const
    {
        return rgba == other.rgba;
    }
    constexpr bool operator!=(const color& other) const
    {
        return *this == other;
    }
    ~color() = default;

    std::string to_string() const
    {
        char buffer[10];
        u8 ur = r * 255, ug = g * 255,
           ub = b * 255, ua = a * 255;

        std::sprintf(buffer, "#%02x%02x%02x%02x", ur, ug, ub, ua);
        return std::string(buffer);
    }
    constexpr std::tuple<u8, u8, u8, u8> to_u8() const
    {
        return std::make_tuple(
            u8(r * 255),
            u8(g * 255),
            u8(b * 255),
            u8(a * 255));
    }
    constexpr u32 to_u32() const
    {
        return u32(a * 255)
               | (u32(b * 255) << 8)
               | (u32(g * 255) << 16)
               | (u32(r * 255) << 24);
    }

    static constexpr f32 normalize(u8 value) { return f32(value) / 255; }
    static constexpr vec4 normalize(const u8vec4& value)
    {
        return vec4{ normalize(value.x), normalize(value.y),
                     normalize(value.z), normalize(value.w) };
    }
};
} // namespace lemon
