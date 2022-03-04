#include <lemon/engine/scene/entity.h>

namespace lemon {
entity::entity(ptr<entity_registry> registry, entity_handle name):
    registry(registry), handle(name)
{
}
void entity::destroy(entity ent)
{
    ent.get_registry()->destroy(ent.handle);
}
} // namespace lemon
