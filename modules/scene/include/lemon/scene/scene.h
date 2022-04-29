#pragma once

#include "entity.h"
#include "system.h"

#include <lemon/core/math/vec2.h>

#include <vector>

namespace lemon {
class LEMON_PUBLIC scene
{
  public:
    scene(string_id name, entity_registry&& registry);
    scene(string_id name);
    ~scene();
    template<class T, class... Args>
    ptr<scene> add_system(Args&&... args);
    void initialize();
    void update();
    void begin();
    void set_main_camera(entity mc);

    const string_id& get_id() const { return name; }

    entity add_entity(string_id name);
    entity add_entity(string_id name, entity parent);
    entity add_entity(string_id name, const vec2& position,
                      const vec2& scale, f32 rotation);
    entity add_entity(string_id name, const vec2& position,
                      const vec2& scale, f32 rotation, entity parent);
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
template<class T, class... Args>
inline ptr<scene> scene::add_system(Args&&... args)
{
    systems.push_back(create_owned<T>(this, std::forward<Args>(args)...));
    return this;
}
} // namespace lemon
