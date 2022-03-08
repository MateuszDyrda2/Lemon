#pragma once

#include "entity.h"
#include "system.h"

#include <lemon/core/math/vec3.h>

#include <vector>

namespace lemon {
class LEMON_PUBLIC scene
{
  public:
    scene(string_id name, entity_registry&& registry);
    scene(string_id name);
    ~scene();
    template<class T>
    inline ptr<scene> add_system()
    {
        systems.push_back(create_owned<T>(this));
        return this;
    }
    void initialize();
    void update();
    void begin();

    const string_id& get_id() const { return name; }

    entity add_entity(string_id name);
    entity add_entity(string_id name, entity parent);
    entity add_entity(string_id name, const vec3& position,
                      const vec3& scale, f32 rotation);
    entity add_entity(string_id name, const vec3& position,
                      const vec3& scale, f32 rotation, entity parent);
    entity clone_entity(entity ent, string_id name);
    void remove_entity(entity& ent);
    entity_registry& get_registry() { return registry; }
    entity get_main_camera() const { return mainCamera; }

  private:
    string_id name;
    entity_registry registry;
    entity mainCamera;
    std::vector<owned<system>> systems;
};
} // namespace lemon
