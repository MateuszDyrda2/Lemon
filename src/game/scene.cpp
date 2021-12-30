#include <river/game/scene.h>

#include <river/core/assert.h>
#include <river/game/basic_components.h>

#include <algorithm>

namespace river {
scene::scene(std::string const& name):
    object(string_id(name)), mainCamera(&registry, string_id("MainCamera")),
    scriptingSystem(create_owned<scripting_system>(registry)),
    renderingSystem(create_owned<rendering_system>(registry)),
    transformSystem(create_owned<transform_system>(registry))
{
}
scene::~scene()
{
}
void scene::initialize()
{
}
void scene::begin()
{
}
void scene::update(float dt)
{
    scriptingSystem->update(registry, dt);
    transformSystem->update(registry);
    renderingSystem->render(registry, mainCamera);
}
entity scene::add_entity(string_id name)
{
    auto ent = registry.create(name);
    registry.emplace<transform>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, transform& parent)
{
    auto ent = registry.create(name);
    registry.emplace<transform>(ent, &parent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const glm::vec3 position,
                         const glm::vec3 scale, f32 rotation)
{
    auto ent = registry.create(name);
    registry.emplace<transform>(ent, position, scale, rotation, nullptr);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const glm::vec3 position,
                         const glm::vec3 scale, f32 rotation, transform& parent)
{
    auto ent = registry.create(name);
    registry.emplace<transform>(ent, position, scale, rotation, &parent);
    return entity(&registry, ent);
}
void scene::remove_entity(string_id ent)
{
    registry.destroy(ent);
}
void scene::remove_entity(entity& ent)
{
    registry.destroy(ent.get_handle());
}
} // namespace river
