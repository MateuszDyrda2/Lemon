#include <lemon/events/event_handler.h>

#include <lemon/core/logger.h>

namespace lemon {
ptr<event_handler> event_handler::handler = nullptr;
event_handler::event_handler():
    events{}
{
    LEMON_ASSERT(!handler);
    handler = this;
    LOG_MESSAGE("Event Handler created");
}
event_handler::~event_handler()
{
    LOG_MESSAGE("Event Handler destroyed");
}
} // namespace lemon
