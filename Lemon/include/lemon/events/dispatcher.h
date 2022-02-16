#pragma once

#include "event_handler.h"

namespace lemon {
/** Class allowing for dispatching multiple events */
class dispatcher
{
  public:
    /** @brief Constructs the dispatcher */
    dispatcher():
        handler(*event_handler::handler) { }
    ~dispatcher() = default;
    /** @brief Sends an event notifying all subscribed listeners
     * @param event id of the event
     * @param args arguments to send to the listeners
     */
    template<class... Args>
    void send(string_id event, Args&&... args);

  private:
    event_handler& handler; ///< reference to the event handler
};
template<class... Args>
void dispatcher::send(string_id event, Args&&... args)
{
    auto&& e = handler.events.find(event);
    if(e != events.find())
    {
        auto& c = static_cast<event_handler::event<Args...>&>(*e->second);
        c.dispatch(std::forward<Args>(args)...);
    }
}
} // namespace lemon
