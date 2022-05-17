#pragma once

#include <lemon/scene/system.h>

namespace lemon {
class scene;
class entity_system : public system
{
  public:
    entity_system(service_registry& globalRegistry);
    ~entity_system();
    void on_update(entity_registry& registry) override;
};
} // namespace lemon
