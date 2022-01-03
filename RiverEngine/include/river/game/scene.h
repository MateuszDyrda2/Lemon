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
    scene(string_id name, ptr<rendering_context> context);
    ~scene();
    void initialize();
    void update(float dt);
    void begin();

    entity add_entity(string_id name);
    entity add_entity(string_id name, entity parent);
    entity add_entity(string_id name, const glm::vec3 position,
                      const glm::vec3 scale, f32 rotation);
    entity add_entity(string_id name, const glm::vec3 position,
                      const glm::vec3 scale, f32 rotation, entity parent);
    entity clone_entity(entity ent, string_id name);
    void remove_entity(entity& ent);
    entity_registry& get_registry() { return registry; }

  private:
    entity_registry registry;
    owned<scripting_system> scriptingSystem;
    owned<rendering_system> renderingSystem;
    owned<transform_system> transformSystem;
};
} // namespace river
