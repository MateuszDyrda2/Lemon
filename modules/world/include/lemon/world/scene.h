#pragma once

#include "entity.h"

#include <lemon/core/hash_string.h>
#include <lemon/core/lemon_types.h>

#include <functional>

namespace lemon {
class scene
{
  public:
    scene(hash_str nameid);
    ~scene();
    void update();

    entity create_entity(hash_str nameid);
    void destroy(entity ent);

    template<class... Args>
    void register_system(const std::function<void(entity, Args&&...)>& system);

  private:
    hash_str nameid;
    entt::registry world;
};
template<class... Args>
void scene::register_system(const std::function<void(entity, Args&&...)>& system)
{
}
} // namespace lemon
