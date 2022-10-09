#include <world/entity.h>

#include <world/components/transform_components.h>
#include <world/scene.h>

namespace lemon {
void entity::set_dirty()
{
    lemon_assert(reg);
    reg->emplace_or_replace<dirty_t>(handle);
}

entity::entity(registry* reg, entity_t handle):
    handle(handle), reg(reg)
{ }

entity::entity(scene& _scene, entity_t handle):
    handle(handle), reg(&_scene.get_registry())
{ }
}
