#pragma once

#include <river/game/entity.h>

namespace river {
class scene;
class scripting_system
{
  public:
    scripting_system(ptr<scene> s);
    ~scripting_system();
    void update(entity_registry& registry, float deltaTime);
};
} // namespace river
