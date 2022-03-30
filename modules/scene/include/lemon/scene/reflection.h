#pragma once

#include <lemon/scene/entity.h>

#include <string>
#include <type_traits>
#include <unordered_map>

#define CONCAT_IMPL(_a1, _a2) _a1##_a2
#define CONCAT(_a1, _a2)      CONCAT_IMPL(_a1, _a2)
#define GET_ARG_COUNT(...) \
    _INTERNAL_GET_ARG_COUNT(0, __VA_ARGS__, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _INTERNAL_GET_ARG_COUNT(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _count, ...) _count

#define LEMON_REFLECT(_class, ...)                                                   \
  private:                                                                           \
    friend class reflection;                                                         \
    static inline struct _refl_impl                                                  \
    {                                                                                \
        _refl_impl()                                                                 \
        {                                                                            \
            detail::_component_def def;                                              \
            def.get_component_f = +[](entity& e) -> component* {                     \
                return static_cast<component*>(&e.get_component<_class>());          \
            };                                                                       \
            def.add_component_f = +[](entity& e, const component& c) {               \
                e.add_component<_class>(static_cast<const _class&>(c));              \
            };                                                                       \
            def.has_component_f = +[](entity& e) {                                   \
                return e.has_component<_class>();                                    \
            };                                                                       \
            reflection::get_registeredComponents()[#_class] = def;                   \
        }                                                                            \
    } _refl_inst;                                                                    \
    using field_n = std::integral_constant<std::size_t, GET_ARG_COUNT(__VA_ARGS__)>; \
    inline static const char* _name()                                                \
    {                                                                                \
        return #_class;                                                              \
    }                                                                                \
    template<class T, std::size_t I>                                                 \
    struct field;                                                                    \
    REFL_N(_class, __VA_ARGS__)                                                      \
  public:

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

#define REFL_N(_comp, ...)                                                                                                      \
    GET_REFL(_0, __VA_ARGS__, REFL_11, REFL_10, REFL_9, REFL_8, REFL_7, REFL_6, REFL_5, REFL_4, REFL_3, REFL_2, REFL_1, REFL_0) \
    (_comp, __VA_ARGS__)
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
struct component
{
    virtual ~component() = default;
};
namespace detail {
struct _component_def
{
    using b_component = component*;
    b_component (*get_component_f)(entity&);
    void (*add_component_f)(entity&, const component&);
    bool (*has_component_f)(entity&);
};
}
class reflection
{
  public:
    static std::unordered_map<std::string, detail::_component_def>&
    get_registeredComponents();

    template<class T>
    static const char* name()
    {
        return T::_name();
    }
    template<class T>
    static constexpr std::size_t field_n()
    {
        return T::field_n::value;
    }
    template<std::size_t I, class T>
    static constexpr decltype(auto) get_field(T& comp)
    {
        using field = typename T::field<T, I>;
        field f(comp);
        return f.get();
    }
    template<std::size_t I, class T>
    static constexpr decltype(auto) get_field(const T& comp)
    {
        using field = typename T::field<const T, I>;
        const field f(comp);
        return f.get();
    }

    template<class T, class F, std::size_t I = 0>
    static void for_each(T& comp, F&& callable) requires(I < T::field_n::value)
    {
        callable(get_field<I, T>(comp));
        for_each<T, F, I + 1>(comp, std::forward<F>(callable));
    }
    template<class T, class F, std::size_t I = 0>
    static void for_each(T&, F&&) requires(I == T::field_n::value)
    { }

    template<class T, class F, std::size_t I = 0>
    static void for_each(const T& comp, F&& callable) requires(I < T::field_n::value)
    {
        callable(get_field<I, T>(comp));
        for_each<T, F, I + 1>(comp, std::forward<F>(callable));
    }
    template<class T, class F, std::size_t I = 0>
    static void for_each(const T&, F&&) requires(I == T::field_n::value) { }
};
} // namespace lemon
