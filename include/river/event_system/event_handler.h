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
    unsubscriber(container_type* l, iterator_type& iter):
        list(l), iterator(iter)
    { }
    void dispose()
    {
        list->erase(iterator);
    }

  private:
    iterator_type iterator;
    container_type* list;
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
        event_base(const std::string& name);
    };
    template<class... Args>
    class event : public event_base
    {
      public:
        using container_type = std::list<listener_type<Args...>>;

      public:
        event(const std::string& name);
        void dispatch(Args&&... args)
        {
            std::for_each(listeners.begin(), listeners.end(), [](const listener_type<Args...>& l) { l(); });
        }
        unsubscriber<Args...> subscribe(const listener_type<Args...>& listener)
        {
            return unsubscriber(&listeners, listeners.insert(listeners.end(), listener));
        }

      private:
        container_type listeners;
    };

  public:
    using map_type = std::unordered_map<string_id, std::unique_ptr<event_base>>;

  public:
    event_handler();
    template<class... Args>
    void register(const std::string& eventName)
    {
        auto e   = std::make_unique<event<Args...>>(eventName);
        auto res = events.insert(std::make_pair(e->get_id(), std::static_pointer_cast<event_base>(e)));
        RIVER_ASSERT(res->second);
    }
    template<class... Args>
    void dispatch(const std::string& eventName, Args&&... args)
    {
        auto& e = events.find(string_id(eventName));
        RIVER_ASSERT(e != events.end());
        auto c = std::static_pointer_cast<event<Args...>>(e->second);
        c->dispatch(std::forward<Args>(args)...);
    }
    template<class... Args>
    unsubscriber<Args...> subscribe(const std::string& eventName, const listener_type<Args...>& listener)
    {
        auto& e = events.find(string_id(eventName));
        RIVER_ASSERT(e != events.end());
        auto c = std::static_pointer_cast<event<Args...>>(e->second);
        return c->subscribe(listener);
    }

  private:
    map_type events;
};
} // namespace river