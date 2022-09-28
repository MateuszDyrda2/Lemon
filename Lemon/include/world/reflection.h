#pragma once

#include "scene.h"

#include <string>
#include <type_traits>
#include <unordered_map>

#define EXP(x)                x
#define CONCAT_IMPL(_a1, _a2) _a1##_a2
#define CONCAT(_a1, _a2)      CONCAT_IMPL(_a1, _a2)

#define GET_ARG_COUNT(...)                                 _NARGS(PREFIX(__VA_ARGS__))
#define _NARGS(...)                                        EXP(LASTOF(__VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
#define LASTOF(a, b, c, d, e, f, g, h, i, j, k, l, m, ...) m
#define PREFIX(...)                                        0, ##__VA_ARGS__

#define LEMON_TAG(_class)                                       \
    template<>                                                  \
    struct reflection::refl<_class>                             \
    {                                                           \
        using field_n = std::integral_constant<std::size_t, 0>; \
        inline static const char* name()                        \
        {                                                       \
            return #_class;                                     \
        }                                                       \
        static constexpr hash_str nameid()                      \
        {                                                       \
            return hash_string(#_class);                        \
        }                                                       \
    };

#define LEMON_REFL(_class, ...)                                                          \
    template<>                                                                           \
    struct reflection::refl<_class>                                                      \
    {                                                                                    \
        using field_n = std::integral_constant<std::size_t, GET_ARG_COUNT(__VA_ARGS__)>; \
        inline static const char* name()                                                 \
        {                                                                                \
            return #_class;                                                              \
        }                                                                                \
        static constexpr hash_str nameid()                                               \
        {                                                                                \
            return hash_string(#_class);                                                 \
        }                                                                                \
        template<class T, std::size_t I>                                                 \
        struct field;                                                                    \
        REFL_N(_class, __VA_ARGS__)                                                      \
    };

#define REFL_FIELD(_class, _field, _i)            \
    template<class T>                             \
    struct field<T, _i>                           \
    {                                             \
        using field_t = decltype(_class::_field); \
        T& inst;                                  \
        field(T& c): inst(c) { }                  \
        inline static const char* name()          \
        {                                         \
            return #_field;                       \
        }                                         \
        inline field_t& get()                     \
        {                                         \
            return inst._field;                   \
        }                                         \
        inline const field_t& get() const         \
        {                                         \
            return inst._field;                   \
        }                                         \
    };

#define REFL_N(_comp, ...)                                                                                                             \
    EXP(GET_REFL(_0, ##__VA_ARGS__, REFL_11, REFL_10, REFL_9, REFL_8, REFL_7, REFL_6, REFL_5, REFL_4, REFL_3, REFL_2, REFL_1, REFL_0)) \
    EXP((_comp, ##__VA_ARGS__))

#define GET_REFL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, NAME, ...) NAME

#define REFL_0(_comp)

#define REFL_1(_comp, _1) \
    REFL_FIELD(_comp, _1, 0)

#define REFL_2(_comp, _1, _2) \
    REFL_FIELD(_comp, _2, 1); \
    REFL_1(_comp, _1)

#define REFL_3(_comp, _1, _2, _3) \
    REFL_FIELD(_comp, _3, 2);     \
    REFL_2(_comp, _1, _2)

#define REFL_4(_comp, _1, _2, _3, _4) \
    REFL_FIELD(_comp, _4, 3);         \
    REFL_3(_comp, _1, _2, _3)

#define REFL_5(_comp, _1, _2, _3, _4, _5) \
    REFL_FIELD(_comp, _5, 4);             \
    REFL_4(_comp, _1, _2, _3, _4)

#define REFL_6(_comp, _1, _2, _3, _4, _5, _6) \
    REFL_FIELD(_comp, _6, 5);                 \
    REFL_5(_comp, _1, _2, _3, _4, _5)

#define REFL_7(_comp, _1, _2, _3, _4, _5, _6, _7) \
    REFL_FIELD(_comp, _7, 6);                     \
    REFL_6(_comp, _1, _2, _3, _4, _5, _6)

#define REFL_8(_comp, _1, _2, _3, _4, _5, _6, _7, _8) \
    REFL_FIELD(_comp, _8, 7);                         \
    REFL_7(_comp, _1, _2, _3, _4, _5, _6, _7)

#define REFL_9(_comp, _1, _2, _3, _4, _5, _6, _7, _8, _9) \
    REFL_FIELD(_comp, _9, 8);                             \
    REFL_8(_comp, _1, _2, _3, _4, _5, _6, _7, _8)

#define REFL_10(_comp, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
    REFL_FIELD(_comp, _10, 9);                                  \
    REFL_9(_comp, _1, _2, _3, _4, _5, _6, _7, _8, _9)

#define REFL_11(_comp, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) \
    REFL_FIELD(_comp, _11, 10);                                      \
    REFL_10(_comp, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10)

namespace lemon {
namespace reflection {
class system
{
  public:
    template<class T>
    inline static const char* name()
    {
        return T::name();
    }

    template<class T>
    inline static constexpr hash_str nameid()
    {
        return T::nameid();
    }
};

template<class T>
struct refl
{
    using field_n = std::integral_constant<std::size_t, 0>;
    inline static const char* name()
    {
        return "";
    }
    static constexpr hash_str nameid()
    {
        return 0;
    }
};

class component
{
  public:
    template<class T>
    inline static const char* name()
    {
        return typename refl<T>::name();
    }

    template<class T>
    static constexpr hash_str nameid()
    {
        return typename refl<T>::nameid();
    }

    template<class T>
    static consteval std::size_t field_n()
    {
        return refl<T>::field_n::value;
    }

    template<std::size_t I, class T>
    static constexpr decltype(auto) get_field(T& comp)
    {
        using field = typename refl<T>::template field<T, I>;
        field f(comp);
        return f.get();
    }

    template<std::size_t I, class T>
    static constexpr decltype(auto) get_field(const T& comp)
    {
        using field = typename refl<T>::template field<const T, I>;
        const field f(comp);
        return f.get();
    }

    template<std::size_t I, class T>
    static const char* get_field_name()
    {
        using field = typename refl<T>::template field<T, I>;
        return field::name();
    }

    template<class T, class F, std::size_t I = 0>
    static void for_each(T& comp, F&& callable) requires(I < refl<T>::field_n::value)
    {
        callable(get_field<I, T>(comp), get_field_name<I, T>());
        for_each<T, F, I + 1>(comp, std::forward<F>(callable));
    }

    template<class T, class F, std::size_t I = 0>
    static void for_each(T& comp, F&& callable) requires(I == refl<T>::field_n::value)
    { }

    template<class T, class F, std::size_t I = 0>
    static void for_each(const T& comp, F&& callable) requires(I < refl<T>::field_n::value)
    {
        callable(get_field<I, T>(comp), get_field_name<I, T>());
        for_each<T, F, I + 1>(comp, std::forward<F>(callable));
    }

    template<class T, class F, std::size_t I = 0>
    static void for_each(const T& comp, F&& callable) requires(I == refl<T>::field_n::value)
    { }
};

}
}
