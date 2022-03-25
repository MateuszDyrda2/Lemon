#pragma once

#include <lemon/scene/entity.h>

#include <string>
#include <unordered_map>

#define CONCAT_IMPL(_a1, _a2) _a1##_a2
#define CONCAT(_a1, _a2)      CONCAT_IMPL(_a1, _a2)

#define COMPONENT_DECL(_name)                                     \
    struct _name;                                                 \
    namespace {                                                   \
    struct CONCAT(_registered_struct, _name)                      \
    {                                                             \
        CONCAT(_registered_struct, _name)                         \
        ()                                                        \
        {                                                         \
            detail::_component_def def;                           \
            def.get_component_f = +[](entity& e) {                \
                return (void*)&e.get_component<_name>();          \
            };                                                    \
            def.add_component_f = +[](entity& e, void* c) {       \
                e.add_component<_name*>(*static_cast<_name*>(c)); \
            };                                                    \
            def.has_component_f = +[](entity& e) {                \
                return e.has_component<_name>();                  \
            };                                                    \
            reflection::_registeredComponents[#_name] = def;      \
        }                                                         \
    } CONCAT(_registered_struct_instance, _name);                 \
    }                                                             \
    struct LEMON_PUBLIC _name

#define LCOMPONENT(_name)                                        \
    struct CONCAT(_registered_struct_, _name)                    \
    {                                                            \
        CONCAT(_registered_struct_, _name)                       \
        ()                                                       \
        {                                                        \
            detail::_component_def def;                          \
            def.get_component_f = +[](entity& e) {               \
                return (void*)&e.get_component<_name>();         \
            };                                                   \
            def.add_component_f = +[](entity& e, void* c) {      \
                e.add_component<_name>(*static_cast<_name*>(c)); \
            };                                                   \
            def.has_component_f = +[](entity& e) {               \
                return e.has_component<_name>();                 \
            };                                                   \
            reflection::_registeredComponents[#_name] = def;     \
        }                                                        \
    } CONCAT(_registered_struct_instance, _name)

#define REFLECT_COMPONENT(_name) \
    static refl<_name> CONCAT(reflect_, _name)(#_name)

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
    void (*add_component_f)(entity&, void*);
    bool (*has_component_f)(entity&);
};
}
class reflection
{
  public:
    static std::unordered_map<std::string, detail::_component_def>&
    get_registeredComponents();
};
template<class T>
struct refl
{
    refl(const std::string& name)
    {
        detail::_component_def def;
        def.get_component_f = +[](entity& e) {
            return (component*)&e.get_component<T>();
        };
        def.add_component_f = +[](entity& e, void* c) {
            e.add_component<T>(*static_cast<T*>(c));
        };
        def.has_component_f = +[](entity& e) {
            return e.has_component<T>();
        };
        reflection::get_registeredComponents()[name] = def;
    }
};

} // namespace lemon
