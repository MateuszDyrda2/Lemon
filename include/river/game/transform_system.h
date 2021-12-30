#pragma once

#include <river/game/basic_components.h>

#include <entt/entt.hpp>

namespace river {
class transform_system
{
  public:
    transform_system(entity_registry& registry);
    ~transform_system();
    void update(entity_registry& registry);
};
} // namespace river