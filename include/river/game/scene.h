#pragma once

#include "entity.h"
#include "transform_system.h"

#include <river/renderer/rendering_system.h>
#include <river/scripting/scripting_system.h>

#include <entt/entt.hpp>

namespace river {
class scene : public object
{
  public:
    scene(const std::string& name);
    ~scene();
    void initialize();
    void update(float dt);
    void begin();

    entity add_entity(string_id name);
    entity add_entity(string_id name, transform& parent);
    entity add_entity(string_id name, const glm::vec3 position,
                      const glm::vec3 scale, f32 rotation);
    entity add_entity(string_id name, const glm::vec3 position,
                      const glm::vec3 scale, f32 rotation, transform& parent);

    void remove_entity(string_id ent);
    void remove_entity(entity& ent);

  private:
    entity_registry registry;
    owned<rendering_system> renderingSystem;
    owned<scripting_system> scriptingSystem;
    owned<transform_system> transformSystem;
    entity mainCamera;
};
} // namespace river
