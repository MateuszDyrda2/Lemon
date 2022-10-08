#pragma once

#include "service_container.h"

namespace lemon {
using entity_t = entt::entity;
using registry = entt::registry;

class LEMON_API system
{
  public:
    virtual void update() { }
    virtual const char* get_name() const = 0;
    virtual hash_str get_nameid() const  = 0;
    virtual ~system()                    = default;
};
}
