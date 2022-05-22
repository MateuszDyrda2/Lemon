#include <lemon/events/event_handler.h>

namespace lemon {
void event_handler::subscribe(ptr<event_listener> sys, string_id eventId)
{
    lemon_assert(sys != nullptr);
    lemon_assert(bool(eventId));
    subscribers[eventId].push_back(sys);
}
} // namespace lemon
