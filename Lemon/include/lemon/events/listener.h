#pragma once

#include "event_handler.h"

namespace lemon {
/** Class for subscribing to a specific type of event */
template<class... Args>
class listener
{
  public:
    using callable_type  = std::function<void(Args...)>;
    using container_type = std::list<callable_type>;
    using iterator_type  = container_type::iterator;
    using self_type      = listener<Args>;

  public:
    /** @brief Creates an empty listener */
    listener();
    /** @brief Creates a listener that executes a callable
     * when the event occurs
     * @param eventName id of the event to subscribe to
     * @param callable callable to be executed when the event fires
     */
    template<class F>
    listener(string_id eventName, F&& callable);
    listener(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    /** @brief Constructs a listener from rvalue
     * @param other listener to be moved
     */
    listener(self_type&& other) noexcept;
    /** @brief Assigns other listener to this
     * @param other listener to be moved
     * @return this
     */
    self_type& operator=(self_type&& other) noexcept;
    ~listener();
    /** @brief Unlinks this listener from the event */
    void dispose();
    /** @brief Swaps two listeners
     * @param other listener to swap with
     */
    void swap(self_type& other);

  private:
    container_type* list;   ///< pointer to the list of the events for unsubscribing
    iterator_type iterator; ///< iterator to this element in the list
};
template<class... Args>
listener<Args...>::listener():
    list{ nullptr }, iterator{}
{
}
template<class... Args>
template<class F>
listener<Args...>::listener(string_id event, F&& callable):
{
    event_handler& handler = *event_handler::handler;
    callable_type l(std::forward<F>(callable)...);
    auto e = handler.events.find(event);
    if(e != handler.events.end())
    {
        auto&& [l, it] = static_cast<event_handler::event<Args...>&>(*e->second)
                             .subscribe(callable);
        list     = l;
        iterator = it;
    }
    else
    {
        auto&& ne      = handler.add_event<Args...>(event);
        auto&& [l, it] = ne.subscribe(callable);
        list           = l;
        iterator       = it;
    }
}
template<class... Args>
listener<Args...>::listener(self_type&& other) noexcept:
    list(other.list), iterator(other.iterator)
{
    other.list = nullptr;
}
template<class... Args>
typename listener<Args...>::self_type&
listener<Args...>::operator=(self_type&& other) noexcept
{
    if(this != other)
    {
        if(list) list->erase();
        list       = other.list;
        iterator   = other.iterator;
        other.list = nullptr;
    }
    return *this;
}
template<class... Args>
listener<Args...>::~listener()
{
    if(list) list->erase(iterator);
}
template<class... Args>
void listener<Args...>::dispose()
{
    list->erase(iterator);
    list = nullptr;
}
template<class... Args>
void listener<Args...>::swap(self_type& other)
{
    std::swap(list, other.list);
    std::swap(iterator, other.iterator);
}
} // namespace lemon
