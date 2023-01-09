/** @file events.h
 * Event queue and event arguments
 */
#pragma once

#include <core/defines.h>
#include <core/hash_string.h>

#include <functional>
#include <list>
#include <queue>
#include <unordered_map>

namespace lemon {
/** Event arguments base */
struct LEMON_API event_args
{
    virtual ~event_args() = default;
};
/** @brief Get event cast as a proper type */
template<class T>
inline static constexpr decltype(auto) get_event(event_args* e)
{
    return *static_cast<T*>(e);
}
/** Event queue for registering and firing events */
class LEMON_API event_queue
{
  public:
    using event_handle = hashstr;
    using callback     = std::function<void(event_args*)>;
    using event_pair   = std::pair<event_handle, event_args*>;

  public:
    class LEMON_API event_sink;
    /** Class for unsubscribing from events */
    class LEMON_API listener_handle
    {
      public:
        listener_handle();
        ~listener_handle();
        listener_handle(const listener_handle&)            = delete;
        listener_handle& operator=(const listener_handle&) = delete;
        listener_handle(listener_handle&& other) noexcept;
        listener_handle& operator=(listener_handle&& other) noexcept;

      private:
        friend event_sink;
        listener_handle(std::list<callback>& listeners,
                        std::list<callback>::iterator iter);
        std::list<callback>* listeners;
        std::list<callback>::iterator iter;
    };
    /** Class allowing for subscribing and firing events */
    class LEMON_API event_sink
    {
      public:
        /** @brief Subscribe to an event
         * @param callable function to be called when an event is fired
         * @return handle for unsubscribing from event
         * @warning listeners handle destructor unsubscribes from the event */
        [[nodiscard]] listener_handle subscribe(const callback& callable);
        /** @brief Subscribe to an event
         * @param callable function to be called when an event is fired
         * @return handle for unsubscribing from event
         * @warning listeners handle destructor unsubscribes from the event */
        [[nodiscard]] listener_handle operator+=(const callback& callable);
        /** @brief Fire the event
         * @param e event arguments */
        void fire(event_args* e);
        /** brief Fire the event synchronously
         * @param e event arguments */
        void fire_immediate(event_args* e = nullptr);

        event_sink(const event_sink&) = delete;

      private:
        friend event_queue;
        event_sink(event_handle handle,
                   std::list<callback>& listeners,
                   std::queue<event_pair>& eventQueue);

      private:
        event_handle handle;
        std::list<callback>& listeners;
        std::queue<event_pair>& eventQueue;
    };

  public:
    event_queue();
    ~event_queue();

    /** @brief Processes all events currently in queue */
    void process();
    /** @brief Returns an event sink
     * @param eventId id of the event to get
     * @return event handler allowing for subscribing and firing events
     */
    [[nodiscard]] event_sink event(event_handle eventId);
    /** @brief Returns an event sink
     * @param eventId id of the event to get
     * @return event handler allowing for subscribing and firing events
     */
    event_sink operator[](event_handle eventId);

  private:
    std::unordered_map<event_handle, std::list<callback>> listeners;
    std::queue<event_pair> eventQueue;
};
} // namespace lemon
