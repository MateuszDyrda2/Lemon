#include <lemon/scene/systems/time_system.h>

namespace lemon {
time_system::time_system()
{ }
time_system::~time_system()
{ }
void time_system::update(entity_registry registry)
{
    gameClock.update();
}
} // namespace lemon
