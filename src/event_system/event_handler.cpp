#include <river/core/logger.h>
#include <river/event_system/event_handler.h>

namespace river {
event_handler::event_handler():
    object("event_handler")
{
    LOG_MESSAGE("Event handler created");
}
event_handler::~event_handler()
{
    LOG_MESSAGE("Event handler destroyed");
}
event_handler& event_handler::get()
{
    static event_handler g;
    return g;
}
} // namespace river
