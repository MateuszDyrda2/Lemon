#include <river/game/entity.h>

namespace river {
entity::entity(ptr<entity_registry> registry, entity_handle name):
    registry(registry), handle(name)
{
}
void entity::destroy(entity ent)
{
    ent.get_registry()->destroy(ent.handle);
}
} // namespace river
