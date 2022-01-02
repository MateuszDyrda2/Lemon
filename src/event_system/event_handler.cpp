#include <river/core/logger.h>
#include <river/event_system/event_handler.h>

namespace river {
event_handler::event_handler():
    events{}
{
    LOG_MESSAGE("Event Handler created");
}
event_handler::~event_handler()
{
    LOG_MESSAGE("Event Handler destroyed");
}
} // namespace river
