#pragma once

#include <lemon/platform/input.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC player_system : public system
{
  public:
    player_system(ptr<scene> s, input& i);
    ~player_system();
    void update(entity_registry& registry) override;

  private:
    input& i;
};
} // namespace lemon
