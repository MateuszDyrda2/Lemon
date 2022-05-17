#include <lemon/scene/systems/event_system.h>

#include <lemon/scene/scene.h>

namespace lemon {
event_system::event_system(ptr<scene> s)
{
}
event_system::~event_system()
{
}
void event_system::subscribe(ptr<event_listener> sys, string_id eventId)
{
    lemon_assert(sys != nullptr);
    lemon_assert(bool(eventId));
    subscribers[eventId].push_back(sys);
}
} // namespace lemon
