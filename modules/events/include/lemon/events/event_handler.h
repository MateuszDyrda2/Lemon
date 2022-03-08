#pragma once

#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>

#include <functional>
#include <list>
#include <unordered_map>

namespace lemon {
struct LEMON_PUBLIC event_base
{
    event_base()          = default;
    virtual ~event_base() = default;
};
class LEMON_PUBLIC event_handler
{
  public:
    event_handler();

    static void fire_event(string_id name, event_base* event);
    static void fire_event(const std::string& name, event_base* event);
    static void subscribe(string_id name, const std::function<void(event_base*)>& fun);
    static void subscribe(const std::string& name, const std::function<void(event_base*)>& fun);

  private:
    std::unordered_map<string_id, std::list<std::function<void(event_base*)>>> reg;
    static event_handler* handler;
};
} // namespace lemon
