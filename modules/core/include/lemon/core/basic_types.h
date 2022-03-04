#pragma once

#include <cstdint>
#include <memory>

namespace lemon {
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

template<class T>
using ref = std::shared_ptr<T>;
template<class T>
using owned = std::unique_ptr<T>;
template<class T>
using ptr = T*;

template<class T>
inline ref<T> create_ref(size_type size) requires std::is_array_v<T>
{
    return std::make_shared<T>(size);
}
template<class T, class... Args>
inline ref<T> create_ref(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}
template<class T>
inline owned<T> create_owned(size_type size) requires std::is_array_v<T>
{
    return std::make_unique<T>(size);
}
template<class T, class... Args>
inline owned<T> create_owned(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}