#pragma once

#include <lemon/scene/system.h>
#include <lemon/threading/scheduler.h>

namespace lemon {
class scene;
class collision_response_system : public system
{
  public:
    collision_response_system(ptr<scene> s, scheduler& sch);
    ~collision_response_system();

    void update(entity_registry& registry) override;

  private:
    scheduler& sch;
};
} // namespace lemon
