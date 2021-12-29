#pragma once

#include <river/core/assert.h>
#include <river/core/basic_types.h>
#include <river/core/string_id.h>
#include <river/game/object.h>

#include <functional>
#include <list>
#include <memory>
#include <unordered_map>

namespace river {
template<class... Args>
class unsubscriber
{
  public:
    using listener_type  = std::function<void(Args...)>;
    using container_type = std::list<listener_type>;
    using iterator_type  = container_type::iterator;

  public:
    unsubscriber() = default;
    unsubscriber(container_type* l, iterator_type iter):
        list(l), iterator(iter)
    { }
    void dispose()
    {
        list->erase(iterator);
    }

  private:
    container_type* list;
    iterator_type iterator;
};
class event_handler : public object
{
  public:
    template<class... Args>
    using listener_type = std::function<void(Args...)>;

  public:
    event_handler();
    ~event_handler();
    template<class... Args>
    static void register_event(const std::string& eventName);
    template<class... Args>
    static void dispatch(const std::string& eventName, Args&&... args);
    template<class... Args>
    static void dispatch(string_id eventId, Args&&... args);
    template<class... Args, class F>
    static unsubscriber<Args...>
    subscribe(const std::string& eventName, F&& callable)
    {
        return subscribe_impl(eventName, listener_type<Args...>(callable));
    }
    template<class... Args>
    static unsubscriber<Args...>
    subscribe_impl(const std::string& eventName, const listener_type<Args...>& listener);

  private:
    class event_base : public object
    {
      public:
        event_base(const std::string& name):
            object(name) { }
    };
    template<class... Args>
    class event : public event_base
    {
      public:
        using container_type = std::list<listener_type<Args...>>;

      public:
        event(const std::string& name):
            event_base(name) { }
        void dispatch(Args&&... args)
        {
            std::for_each(listeners.begin(), listeners.end(),
                          [&args...](const listener_type<Args...>& l) {
                              l(std::forward<Args>(args)...);
                          });
        }
        unsubscriber<Args...> subscribe(const listener_type<Args...>& listener)
        {
            return unsubscriber<Args...>(&listeners, listeners.insert(listeners.end(), listener));
        }

      private:
        container_type listeners;
    };

  public:
    using map_type = std::unordered_map<string_id, std::unique_ptr<event_base>>;

  private:
    static event_handler& get();
    map_type events;
};
template<class... Args>
void event_handler::register_event(const std::string& eventName)
{
    auto& handler = get();
    auto e        = std::make_unique<event<Args...>>(eventName);
    auto res      = handler.events.insert(std::make_pair(e->get_id(), std::move(e)));
    RIVER_ASSERT(res.second);
}
template<class... Args>
void event_handler::dispatch(const std::string& eventName, Args&&... args)
{
    dispatch(string_id(eventName), std::forward<Args>(args)...);
}
template<class... Args>
void event_handler::dispatch(string_id eventId, Args&&... args)
{
    auto& handler = get();
    auto e        = handler.events.find(eventId);
    RIVER_ASSERT(e != handler.events.end());
    auto& c = static_cast<event<Args...>&>(*(e->second));
    c.dispatch(std::forward<Args>(args)...);
}
template<class... Args>
unsubscriber<Args...> event_handler::subscribe_impl(
    const std::string& eventName, const listener_type<Args...>& listener)
{
    auto& handler = get();
    auto e        = handler.events.find(string_id(eventName));
    RIVER_ASSERT(e != handler.events.end());
    auto& c = static_cast<event<Args...>&>(*(e->second));
    return c.subscribe(listener);
}

} // namespace river