#pragma once

#include <lemon/core/assert.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/string_id.h>
#include <lemon/game/object.h>
#include <lemon/service/service.h>

#include <functional>
#include <list>
#include <memory>
#include <unordered_map>

namespace lemon {
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
    unsubscriber(const unsubscriber&) = delete;
    unsubscriber& operator=(const unsubscriber&) = delete;
    unsubscriber(unsubscriber&& other) noexcept:
        list(other.list), iterator(other.iterator)
    {
        other.list = nullptr;
    }
    unsubscriber& operator=(unsubscriber&& other) noexcept
    {
        if(this != &other)
        {
            std::swap(list, other.list);
            std::swap(iterator, other.iterator);
        }
        return *this;
    }
    void dispose()
    {
        list->erase(iterator);
        list = nullptr;
    }
    ~unsubscriber()
    {
        if(list) list->erase(iterator);
    }

  private:
    container_type* list{};
    iterator_type iterator;
};
class event_handler : public service
{
  public:
    template<class... Args>
    using listener_type = std::function<void(Args...)>;

  public:
    event_handler();
    ~event_handler();
    template<class... Args>
    void dispatch(string_id eventName, Args&&... args);
    template<class... Args, class F>
    [[nodiscard]] unsubscriber<Args...>
    subscribe(string_id eventName, F&& callable);

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
        unsubscriber<Args...> subscribe(const listener_type<Args...>& listener)
        {
            return unsubscriber<Args...>(&listeners, listeners.insert(listeners.end(), listener));
        }

      private:
        container_type listeners;
    };

  public:
    using map_type = std::unordered_map<string_id, owned<object>>;

  private:
    map_type events;
};
template<class... Args>
void event_handler::dispatch(string_id eventId, Args&&... args)
{
    auto e = events.find(eventId);
    if(e != events.end())
    {
        auto& c = static_cast<event<Args...>&>(*(e->second));
        c.dispatch(std::forward<Args>(args)...);
    }
}
template<class... Args, class F>
unsubscriber<Args...> event_handler::subscribe(
    string_id eventName, F&& callable)
{
    listener_type<Args...> listener(std::forward<F>(callable));
    auto e = events.find(eventName);
    if(e != events.end())
    {
        auto& c = static_cast<event<Args...>&>(*(e->second));
        return c.subscribe(listener);
    }
    else
    {
        auto ne  = create_owned<event<Args...>>(eventName);
        auto res = events.insert(std::make_pair(ne->get_id(), std::move(ne)));
        LEMON_ASSERT(res.second);
        return static_cast<event<Args...>*>((*res.first).second.get())->subscribe(listener);
    }
}

} // namespace lemon