#include <events/events.h>

namespace lemon {
event_queue::listener_handle::listener_handle():
    listeners{}, iter{}
{ }
event_queue::listener_handle::listener_handle(std::list<callback>& listeners,
                                              std::list<callback>::iterator iter):
    listeners(&listeners),
    iter(iter)
{ }
event_queue::listener_handle::~listener_handle()
{
    if(listeners)
    {
        listeners->erase(iter);
    }
}
event_queue::listener_handle::listener_handle(listener_handle&& other) noexcept:
    listeners(other.listeners), iter(other.iter)
{
    other.listeners = nullptr;
}
event_queue::listener_handle& event_queue::listener_handle::operator=(listener_handle&& other) noexcept
{
    if(this != &other)
    {
        std::swap(listeners, other.listeners);
        std::swap(iter, other.iter);
    }
    return *this;
}
event_queue::listener_handle event_queue::event_sink::subscribe(const callback& callable)
{
    listeners.push_back(callable);
    return listener_handle(listeners, std::prev(listeners.end()));
}
event_queue::listener_handle event_queue::event_sink::operator+=(const callback& callable)
{
    listeners.push_back(callable);
    return listener_handle(listeners, std::prev(listeners.end()));
}
void event_queue::event_sink::fire(event_args* e)
{
    eventQueue.push({ handle, e });
}
event_queue::event_sink::event_sink(event_handle handle,
                                    std::list<callback>& listeners,
                                    std::queue<event_pair>& eventQueue):
    handle(handle),
    listeners(listeners),
    eventQueue(eventQueue)
{ }

event_queue::event_queue()
{
    logger::info("Event queue created");
}
event_queue::~event_queue()
{
    logger::info("Event queue destroyed");
}
void event_queue::process()
{
    while(!eventQueue.empty())
    {
        auto&& front = eventQueue.front();
        auto& l      = listeners[front.first];
        std::for_each(l.begin(), l.end(),
                      [&](auto& callback) {
                          callback(front.second);
                      });
        delete front.second;
        eventQueue.pop();
    }
}
event_queue::event_sink event_queue::event(event_handle eventId)
{
    return event_sink(eventId, listeners[eventId], eventQueue);
}
event_queue::event_sink event_queue::operator[](event_handle eventId)
{
    return event_sink(eventId, listeners[eventId], eventQueue);
}
} // namespace lemon
