#pragma once

#include <assets/asset.h>
#include <events/events.h>
#include <platform/window.h>
#include <threading/scheduler.h>

#include <entt/entt.hpp>

#include <unordered_map>

#define CONCAT(arg1, arg2) arg1##arg2
#define ARG_NAME(arg)      CONCAT(_, arg)
#define LOCATOR_NAME(arg)  CONCAT(locator., ARG_NAME(arg))

#define ARGUMENT(arg) CONCAT(arg&, ARG_NAME(arg))

#define SYSTEM0(_NAME)                                                          \
    _NAME(service_container&& locator): _NAME()                                 \
    {                                                                           \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create; \
    }                                                                           \
    _NAME()

#define SYSTEM1(_NAME, _1)                                                      \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1))                 \
    {                                                                           \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create; \
    }                                                                           \
    _NAME(ARGUMENT(_1))

#define SYSTEM2(_NAME, _1, _2)                                                    \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2)) \
    {                                                                             \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create;   \
    }                                                                             \
    _NAME(ARGUMENT(_1), ARGUMENT(_2))

#define SYSTEM3(_NAME, _1, _2, _3)                                                                  \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3)) \
    {                                                                                               \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create;                     \
    }                                                                                               \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3))

#define SYSTEM4(_NAME, _1, _2, _3, _4)                                                                                \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3), LOCATOR_NAME(_4)) \
    {                                                                                                                 \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create;                                       \
    }                                                                                                                 \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3), ARGUMENT(_4))

#define SYSTEM5(_NAME, _1, _2, _3, _4, _5)                                                                                              \
    _NAME(service_container&& locator): _NAME(LOCATOR_NAME(_1), LOCATOR_NAME(_2), LOCATOR_NAME(_3), LOCATOR_NAME(_4), LOCATOR_NAME(_5)) \
    {                                                                                                                                   \
        detail::system_registry::systems[hash_string(#_NAME)] = &_NAME::create;                                                         \
    }                                                                                                                                   \
    _NAME(ARGUMENT(_1), ARGUMENT(_2), ARGUMENT(_3), ARGUMENT(_4), ARGUMENT(_5))

#define GET_MACRO(_0, _1, _2, _3, _4, _5, NAME, ...) NAME
#define EXPAND(x)                                    x
#define SYSTEM(_NAME, ...)                                                                              \
    inline static system* create(service_container&& locator) { return new _NAME(std::move(locator)); } \
    EXPAND(GET_MACRO(_0, __VA_ARGS__, SYSTEM5, SYSTEM4, SYSTEM3, SYSTEM2, SYSTEM1, SYSTEM0))            \
    EXPAND((_NAME, __VA_ARGS__))

namespace lemon {
struct LEMON_API service_container
{
    asset_storage& _asset_storage;
    scheduler& _scheduler;
    event_queue& _event_queue;
    entt::registry& _registry;
    window& _window;
};

class system;
namespace detail {
struct LEMON_API system_registry
{
    using func = system* (*)(service_container&&);
    static inline std::unordered_map<hash_str, func> systems;
};
};
}
