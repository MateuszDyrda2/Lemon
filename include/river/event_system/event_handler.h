#pragma once

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
    unsubscriber(container_type& l, iterator_type& iter):
        list(l), iterator(iter)
    { }
    void dispose()
    {
        list.erase(iterator);
    }

  private:
    iterator_type iterator;
    container_type& list;
};
class event_handler : public object
{
  public:
    template<class... Args>
    using listener_type = std::function<void(Args...)>;

  private:
    class event_base : public object
    {
      public:
        event_base(std::string const& name);
    };
    template<class... Args>
    class event : public event_base
    {
      public:
        using container_type = std::list<listener_type<Args...>>;

      public:
        event(std::string const& name);
        void dispatch(Args&&... args)
        {
            std::for_each(listeners.begin(), listeners.end(), [](listener_type<Args...> const& l) { l(); });
        }
        unsubscriber<Args...> subscribe(listener_type<Args...> const& listener)
        {
            return unsubscriber(listeners, listeners.insert(listeners.end(), listener));
        }

      private:
        container_type listeners;
    };

  public:
    using map_type = std::unordered_map<string_id, std::unique_ptr<event_base>>;

  public:
    event_handler();
    template<class... Args>
    void register(std::string eventName)
    {
        auto e = std::make_unique<event<Args...>>(eventName);
        events.insert(std::make_pair(e->get_id(), e));
    }
    template<class... Args>
    void dispatch(std::string eventName, Args&&... args)
    {
        auto& e = events.at(string_id(eventName));
        auto& c = std::static_pointer_cast<event<Args...>>(e);
        c->dispatch(std::forward<Args>(args)...);
    }
    template<class... Args>
    unsubscriber<Args...> subscribe(std::string eventName, listener_type<Args...> const& listener)
    {
        auto& e = events.at(string_id(eventName));
        auto& c = std::static_pointer_cast<event<Args...>>(e);
        return c->subscribe(listener);
    }

  private:
    map_type events;
};

} // namespace river
