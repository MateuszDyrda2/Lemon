#pragma once

#include "entity.h"

namespace lemon {
class LEMON_PUBLIC system
{
  public:
    system()                                       = default;
    virtual ~system()                              = default;
    virtual void update(entity_registry& registry) = 0;
};
} // namespace lemon
