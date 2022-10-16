#pragma once

#include "service_container.h"

namespace lemon {
using entity_t = entt::entity;
using registry = entt::registry;

struct LEMON_API update_event : public event_args
{
    update_event(f32 deltaTime, f32 fixedDelta, f32 alpha):
        deltaTime(deltaTime), fixedDelta(fixedDelta), alpha(alpha) { }

    f32 deltaTime;
    f32 fixedDelta;
    f32 alpha;
};

class LEMON_API system
{
  public:
    virtual const char* get_name() const = 0;
    virtual hash_str get_nameid() const  = 0;
    virtual ~system()                    = default;
};
}
