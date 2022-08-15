#pragma once

#include "../components/entity_components.h"
#include "../system.h"

namespace lemon {
class LEMON_API entity_system : public system
{
  public:
    SYSTEM(entity_system, registry);
    ~entity_system();
    void update() override;

  private:
    registry& _registry;
};
}
