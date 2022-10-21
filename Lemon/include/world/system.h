#pragma once

#include "service_container.h"

namespace lemon {
using entity_t = entt::entity;
using registry = entt::registry;

struct LEMON_API update_event : public event_args
{
    update_event(f32 deltaTime, f32 alpha):
        deltaTime(deltaTime), alpha(alpha) { }

    f32 deltaTime;
    f32 alpha;
};

struct LEMON_API fixed_update_event : public event_args
{
    fixed_update_event(f32 fixedDelta):
        fixedDelta(fixedDelta) { }

    f32 fixedDelta;
};

class LEMON_API system
{
  public:
    virtual const char* get_name() const = 0;
    virtual hash_str get_nameid() const  = 0;
    virtual ~system()                    = default;
};
}
