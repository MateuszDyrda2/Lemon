#pragma once

#include "service_container.h"

namespace lemon {
using entity   = entt::entity;
using registry = entt::registry;

class LEMON_API system
{
  public:
    virtual void update() { }
    virtual ~system() = default;
};
}
