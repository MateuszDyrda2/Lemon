#include <lemon/core/logger.h>
#include <lemon/event_system/event_handler.h>

namespace lemon {
event_handler::event_handler():
    events{}
{
    LOG_MESSAGE("Event Handler created");
}
event_handler::~event_handler()
{
    LOG_MESSAGE("Event Handler destroyed");
}
} // namespace lemon
