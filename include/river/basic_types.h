#pragma once

#include <cstdint>

namespace river {
// basic size type
using size_type = std::size_t;
// basic integer types
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
// byte
using byte = unsigned char;
// floating point
using f32 = float;
using f64 = double;
}