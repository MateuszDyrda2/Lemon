#pragma once

#include "../system.h"

#include <lemon/core/assert.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>

#include <cstring>
#include <queue>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace lemon {
class scene;
using event = std::aligned_storage_t<32, alignof(string_id)>;

class event_system;
class event_listener
{
  protected:
    std::queue<event> eventQueue;
    friend event_system;
};
class LEMON_PUBLIC event_system : public system
{
  public:
    event_system(ptr<scene> s);
    ~event_system();

    template<class... Args>
    requires(sizeof(std::tuple<string_id, Args...>) <= 32) void push_event(
        string_id eventId, Args&&... args) noexcept;
    void subscribe(ptr<event_listener> sys, string_id eventId);

    template<class... Args>
    static const std::tuple<Args...>& get_arguments(event& e);

  private:
    std::unordered_map<string_id, std::vector<ptr<event_listener>>> subscribers;
};
template<class... Args>
requires(sizeof(std::tuple<string_id, Args...>) <= 32) void event_system::push_event(
    string_id eventId, Args&&... args) noexcept
{
    lemon_assert(bool(eventId));
    // does the event have any listeners?
    if(auto iter = subscribers.find(eventId);
       iter != subscribers.end())
    {
        // enqueue event in appropriate systems
        for(auto& listener : iter->second)
        {
            event newEvent;
            ::new(&newEvent) std::tuple<string_id, Args...>(eventId, args...);
            listener->eventQueue.push(std::move(newEvent));
        }
    }
    // else discard the event
}
template<class... Args>
inline const std::tuple<Args...>& get_arguments(event& e)
{
    return *static_cast<std::tuple<Args...>*>(&e)
}
} // namespace lemon
