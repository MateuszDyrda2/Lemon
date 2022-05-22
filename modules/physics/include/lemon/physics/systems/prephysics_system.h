#pragma once

#include <lemon/scene/system.h>

namespace lemon {
class prephysics_system : public system
{
  public:
    prephysics_system(service_registry& globalRegistry);
    ~prephysics_system();
    void on_update(entity_registry& registry) override;
};
} // namespace lemon
