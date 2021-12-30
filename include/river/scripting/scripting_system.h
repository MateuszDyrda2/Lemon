#pragma once

#include <river/game/entity.h>

namespace river {
class scripting_system
{
  public:
    scripting_system(entity_registry& registry);
    ~scripting_system();
    void update(entity_registry& registry, float deltaTime);
};
} // namespace river
