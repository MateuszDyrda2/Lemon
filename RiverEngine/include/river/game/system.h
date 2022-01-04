#pragma once

#include "entity.h"

namespace river {
class system
{
  public:
    system()                                       = default;
    virtual ~system()                              = default;
    virtual void update(entity_registry& registry) = 0;
};
} // namespace river
