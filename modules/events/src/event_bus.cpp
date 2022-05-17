#include <lemon/events/event_bus.h>

#include <lemon/core/logger.h>

namespace lemon {
event_bus::event_sink event_bus::sink(string_id event_id)
{
    listenerLock.lock();
    return event_sink(*this, listeners[event_id]);
}
void event_bus::event_sink::add_listener(listener* l)
{
    listeners.push_back(l);
}
void event_bus::event_sink::remove_listener(listener* l)
{
    listeners.erase(std::find(listeners.begin(), listeners.end(), l));
}
event_bus::event_sink::~event_sink()
{
    ebus.listenerLock.unlock();
}
void event_bus::fire(event* e)
{
    std::lock_guard lck(eventLock);
    events.push_back(e);
}
void event_bus::fire_immediate(event* e)
{
    std::lock_guard lck(listenerLock);
    if(auto&& iter = listeners.find(e->name); iter != listeners.end())
    {
        for(auto&& l : iter->second)
        {
            l->on_event(e);
        }
    }
}
void event_bus::flush()
{
    for(auto&& e : events)
    {
        if(auto&& iter = listeners.find(e->name); iter != listeners.end())
        {
            for(auto&& l : iter->second)
            {
                l->on_event(e);
            }
        }
        delete e;
    }
    events.clear();
}
} // namespace lemon
