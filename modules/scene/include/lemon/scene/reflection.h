#pragma once

#include <lemon/scene/entity.h>

#include <string>
#include <unordered_map>

#define CONCAT_IMPL(_a1, _a2) _a1##_a2
#define CONCAT(_a1, _a2)      CONCAT_IMPL(_a1, _a2)

#define COMPONENT_DECL(_name)                                   \
    struct _name;                                               \
    namespace {                                                 \
    struct CONCAT(_registered_struct, _name)                    \
    {                                                           \
        CONCAT(_registered_struct, _name)                       \
        ()                                                      \
        {                                                       \
            detail::_component_def def;                         \
            def.get_component_f = +[](entity& e) {              \
                return (void*)&e.get_component<_name>();        \
            };                                                  \
            def.add_component_f = +[](entity& e, void* c) {     \
                e.add_component<_name>(static_cast<_name>(*c)); \
            };                                                  \
            def.has_component_f = +[](entity& e) {              \
                return e.has_component<_name>();                \
            };                                                  \
            detail::_registeredComponents[_name] = def;         \
        }                                                       \
    } CONCAT(_registered_struct_instance, _name);               \
    }                                                           \
    struct _name

namespace lemon::detail {
struct _component_def
{
    (void*)(*)(entity&)get_component_f;
    void (*)(entity&, void*) add_component_f;
    bool (*)(entity&) has_component_h;
};
static std::unordered_map<std::string, _component_def> _registeredComponents;
void* get_component(entity& ent, const std::string& name)
{
    return _registeredComponents[name].get_component_f(ent);
}
void add_component(entity& ent, const std::string& name, void* component)
{
    _registeredComponents[name].add_component_f(ent, component);
}
bool has_component(entity& ent, const std::string name)
{
    return _registeredComponents[name].has_component(ent);
}
} // namespace lemon
