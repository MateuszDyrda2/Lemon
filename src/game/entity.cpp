#include <river/game/entity.h>

namespace river {
entity::entity(ptr<entity_registry> registry, string_id name):
    registry(registry), handle(registry->create(name))
{
}
} // namespace river
