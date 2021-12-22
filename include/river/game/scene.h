#pragma once

#include "entity.h"

namespace river {
class scene : public object
{
  public:
    scene(std::string const& name);
    ~scene();

    void update(float deltaTime);
    void fixed_update(float fixedDelta);
    void late_update(float deltaTime);

    std::shared_ptr<entity> const& add_entity(std::shared_ptr<entity> const& ent);
    std::shared_ptr<entity> const& add_entity(std::string const& name);

  private:
    std::vector<std::shared_ptr<entity>> entities;
    std::vector<std::shared_ptr<entity>> toDestroy;
};
} // namespace river
