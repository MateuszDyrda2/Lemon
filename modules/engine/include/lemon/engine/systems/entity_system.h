#pragma once

#include <lemon/scene/system.h>

namespace lemon {
class scene;
class entity_system : public system
{
  public:
    entity_system(ptr<scene> s);
    ~entity_system();
    void update(entity_registry& registry) override;
};
} // namespace lemon
