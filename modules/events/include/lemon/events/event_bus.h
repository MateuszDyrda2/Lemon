#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/service.h>
#include <lemon/core/string_id.h>

#include <functional>
#include <mutex>
#include <unordered_map>

namespace lemon {
class service_registry;
struct LEMON_PUBLIC event
{
    string_id name;
    event(string_id name):
        name(name) { }
};
class LEMON_PUBLIC listener
{
  public:
    virtual void on_event(event*) = 0;
};
class LEMON_PUBLIC event_bus : public service
{
  public:
    LEMON_REGISTER_SERVICE(event_bus);
    using listeners_t = std::unordered_map<string_id, std::vector<listener*>>;
    class event_sink
    {
      public:
        void add_listener(listener* l);
        void remove_listener(listener* l);
        event_sink(const event_sink&) = delete;
        event_sink(event_sink&&)      = delete;
        event_sink& operator=(const event_sink&) noexcept = delete;
        event_sink& operator=(event_sink&&) noexcept = delete;
        inline void operator+=(listener* l)
        {
            add_listener(l);
        }
        inline void operator-=(listener* l)
        {
            remove_listener(l);
        }
        ~event_sink();

      private:
        event_sink(event_bus& b, std::vector<listener*>& l):
            listeners(l), ebus(b) { }

      private:
        friend event_bus;
        std::vector<listener*>& listeners;
        event_bus& ebus;
    };

  public:
    event_bus(service_registry&);
    ~event_bus() = default;
    void fire(event* e);
    void fire_immediate(event* e);
    void flush();
    event_sink sink(string_id event_id);

  private:
    std::vector<event*> events;
    std::mutex eventLock;
    listeners_t listeners;
    std::mutex listenerLock;
    friend event_sink;
};
} // namespace lemon
