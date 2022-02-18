#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/game/object.h>

#include <functional>
#include <list>
#include <unordered_map>

namespace lemon {
/** event handler for managing events */
class event_handler
{
  public:
    template<class... Args>
    using listener_type = std::function<void(Args...)>;

  private:
    template<class... Args>
    class event : public object
    {
      public:
        using container_type = std::list<listener_type<Args...>>;

      public:
        event(string_id name):
            object(name) { }
        void dispatch(Args&&... args)
        {
            std::for_each(listeners.begin(), listeners.end(),
                          [&args...](const listener_type<Args...>& l) {
                              l(std::forward<Args>(args)...);
                          });
        }
        auto subscribe(const listener_type<Args...>& listener)
        {
            return std::make_pair(&listeners, listeners.insert(listeners.end(), listener));
        }

      private:
        container_type listeners;
    };

  public:
    /** @brief Creates an event handler */
    event_handler();
    ~event_handler();
    /** @brief Registers an event for dispatching
     * @param name id of the event
     * @return reference to the registered event
     */
    template<class... Args>
    event<Args...>& add_event(string_id name);

  public:
    using map_type = std::unordered_map<string_id, owned<object>>;

  private:
    map_type events;
    static ptr<event_handler> handler;

    template<class... Args>
    friend class listener;
    friend class dispatcher;
};
template<class... Args>
event_handler::event<Args...>&
event_handler::add_event(string_id name)
{
    auto res = events.insert(std::make_pair(name, create_owned<event<Args...>>(name)));
    return static_cast<event<Args...>&>(*((*res.first).second.get()));
}

} // namespace lemon