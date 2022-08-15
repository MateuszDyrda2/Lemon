#pragma once

#include "../components/transform_components.h"
#include "../system.h"

namespace lemon {
class LEMON_API transform_system : public system
{
  public:
    SYSTEM(transform_system, registry);
    ~transform_system();
    void update() override;

  private:
    registry& _registry;
};
}
