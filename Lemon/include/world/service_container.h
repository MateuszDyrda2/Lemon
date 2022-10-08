#pragma once

#include <assets/asset.h>
#include <core/defines.h>
#include <events/events.h>
#include <platform/input.h>
#include <platform/window.h>
#include <threading/scheduler.h>

#include <entt/entt.hpp>

#include <unordered_map>

#define ARG_NAME(arg)     CONCAT(_, arg)
#define LOCATOR_NAME(arg) locator.ARG_NAME(arg)

#define ARGUMENT(arg) arg& ARG_NAME(arg)

#define SYSTEM0(_NAME)                              \
    _NAME(service_container&& locator): _NAME() { } \
    _NAME()

#define SYSTEM1(_NAME, _1)                                          \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1)) { } \
    _NAME(ARGUMENT(_1))

#define SYSTEM2(_NAME, _1, _2)                                                        \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2)) { } \
    _NAME(ARGUMENT(_1), ARGUMENT(_2))

#define SYSTEM3(_NAME, _1, _2, _3)                                                                      \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3)) { } \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3))

#define SYSTEM4(_NAME, _1, _2, _3, _4)                                                                                    \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3), LOCATOR_NAME(_4)) { } \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3), ARGUMENT(_4))

#define SYSTEM5(_NAME, _1, _2, _3, _4, _5)                                                                                                  \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3), LOCATOR_NAME(_4), LOCATOR_NAME(_5)) { } \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3), ARGUMENT(_4), ARGUMENT(_5))

#define GET_MACRO(_0, _1, _2, _3, _4, _5, NAME, ...) NAME
#define SYSTEM(_NAME, ...)                                                            \
    struct refl                                                                       \
    {                                                                                 \
        refl()                                                                        \
        {                                                                             \
            detail::system_registry::systems()[hash_string(#_NAME)] = &_NAME::create; \
        }                                                                             \
    } _refl;                                                                          \
    inline static const char* name()                                                  \
    {                                                                                 \
        return #_NAME;                                                                \
    }                                                                                 \
    inline static constexpr hash_str nameid()                                         \
    {                                                                                 \
        return hash_string(#_NAME);                                                   \
    }                                                                                 \
    const char* get_name() const override                                             \
    {                                                                                 \
        return name();                                                                \
    }                                                                                 \
    hash_str get_nameid() const override                                              \
    {                                                                                 \
        return nameid();                                                              \
    }                                                                                 \
    inline static system* create(service_container&& locator)                         \
    {                                                                                 \
        return new _NAME(std::move(locator));                                         \
    }                                                                                 \
    GET_MACRO(_0, __VA_ARGS__, SYSTEM5, SYSTEM4, SYSTEM3, SYSTEM2, SYSTEM1, SYSTEM0)  \
    (_NAME, __VA_ARGS__)

namespace lemon {
struct LEMON_API service_container
{
    asset_storage& _asset_storage;
    scheduler& _scheduler;
    event_queue& _event_queue;
    entt::registry& _registry;
    window& _window;
    input& _input;
};

class system;
namespace detail {
struct LEMON_API system_registry
{
    using func = system* (*)(service_container&&);
    static inline auto& systems()
    {
        static std::unordered_map<hash_str, func> _systems;
        return _systems;
    }
};
};
}
