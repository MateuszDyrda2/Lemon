#pragma once

#include <lemon/platform/input.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC input_system : public system
{
  public:
    input_system(ptr<scene> s, input& i);
    ~input_system();
    void update(entity_registry& registry) override;

  private:
    input& i;
};
} // namespace lemon
