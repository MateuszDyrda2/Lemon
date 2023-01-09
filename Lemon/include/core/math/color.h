/** @file color.h
 * @brief file containing a helper class representing a color
 */
#pragma once

#include "vec3.h"
#include "vec4.h"

#include <core/assert.h>
#include <core/defines.h>
#include <core/lemon_types.h>

#include <cstddef>
#include <string>
#include <tuple>
#include <utility>

namespace lemon {
/** color representation */
struct LEMON_API color
{
    union
    {
        struct
        {
            f32 r, g, b, a;
        };
        vec4 rgba;
    };
    /** @return color object representing red */
    static constexpr color red() { return color(1.f, 0.f, 0.f, 1.f); }
    /** @return color object representing green */
    static constexpr color green() { return color(0.f, 1.f, 0.f, 1.f); }
    /** @return color object representing blue */
    static constexpr color blue() { return color(0.f, 0.f, 1.f, 1.f); }
    /** @return color object representing white */
    static constexpr color white() { return color(1.f, 1.f, 1.f, 1.f); }
    /** @return color object representing black */
    static constexpr color black() { return color(0.f, 0.f, 0.f, 1.f); }

    /** @brief Creates a basic black color */
    constexpr color():
        rgba(0.f, 0.f, 0.f, 1.f) { }
    /** @brief Creates a color from rgb values vector
     * @param rgb vector containing normalized color values
     */
    constexpr color(const vec3& rgb):
        rgba{ rgb.x, rgb.y, rgb.z, 1.f } { }
    /** @brief Creates a color from rgba values vector
     * @param rgba vector containing normalized color values
     */
    constexpr color(const vec4& rgba):
        rgba(rgba) { }
    /** @brief Creates a color from rgb values
     * @param r normalized red value
     * @param g normalized green value
     * @param b normalized blue value
     */
    constexpr color(f32 red, f32 green, f32 blue):
        r(red), g(green), b(blue), a(1.f) { }
    /** @brief Creates a color from rgba values
     * @param r normalized red value
     * @param g normalized green value
     * @param b normalized blue value
     * @param a normalized alpha value
     */
    constexpr color(f32 red, f32 green, f32 blue, f32 alpha):
        r(red), g(green), b(blue), a(alpha) { }
    /** @brief Creates a color from rgb values vector
     * @param rgb vector containing 0-255 color values
     */
    constexpr color(const u8vec3& rgb):
        rgba{ normalize(rgb.x), normalize(rgb.y), normalize(rgb.z), 1.f } { }
    /** @brief Creates a color from rgba values vector
     * @param rgba vector containing 0-255 color values
     */
    constexpr color(const u8vec4& rgba):
        rgba(normalize(rgba)) { }
    /** @brief Creates a color from rgb values
     * @param r (0-255) red value
     * @param g (0-255) green value
     * @param b (0-255) blue value
     */
    constexpr color(u8 red, u8 green, u8 blue):
        r(normalize(red)), g(normalize(green)),
        b(normalize(blue)), a(1.f) { }
    /** @brief Creates a color from rgba values
     * @param r (0-255) red value
     * @param g (0-255) green value
     * @param b (0-255) blue value
     * @param a (0-255) alpha value
     */
    constexpr color(u8 red, u8 green, u8 blue, u8 alpha):
        r(normalize(red)), g(normalize(green)),
        b(normalize(blue)), a(normalize(alpha)) { }
    /** @brief Creates a color from a string containing hexadecimal color representation
     * @param hex string in a format \"#000000FF\"
     */
    color(const char* hex)
    {
        int ir, ig, ib, ia;
        lemon_assert(hex[0] == '#');
        std::sscanf(hex, "%*c%02x%02x%02x%02x", &ir, &ig, &ib, &ia);
        rgba = vec4{ normalize(u8(ir)), normalize(u8(ig)), normalize(u8(ib)), normalize(u8(ia)) };
    }
    /** @brief Creates a color from a number
     * @param hex number interpreted as 0x000000FF
     */
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

    /** @return color as a string like "#12FFAAFF" */
    std::string to_string() const
    {
        char buffer[10];
        u8 ur = u8(r * 255), ug = u8(g * 255),
           ub = u8(b * 255), ua = u8(a * 255);

        std::sprintf(buffer, "#%02x%02x%02x%02x", ur, ug, ub, ua);
        return std::string(buffer);
    }
    /** @return color as a 4 value unsigned int tuple */
    constexpr std::tuple<u8, u8, u8, u8> to_u8() const
    {
        return std::make_tuple(
            u8(r * 255),
            u8(g * 255),
            u8(b * 255),
            u8(a * 255));
    }
    /** @return color as one number like "0x0F0FF0FF" */
    constexpr u32 to_u32() const
    {
        return u32(a * 255)
               | (u32(b * 255) << 8)
               | (u32(g * 255) << 16)
               | (u32(r * 255) << 24);
    }
    /** @return normalized value */
    static constexpr f32 normalize(u8 value) { return f32(value) / 255; }
    /** @return normalized value */
    static constexpr vec4 normalize(const u8vec4& value)
    {
        return vec4{ normalize(value.x), normalize(value.y),
                     normalize(value.z), normalize(value.w) };
    }
};
} // namespace lemon
