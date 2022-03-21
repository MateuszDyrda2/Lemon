#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/string_id.h>

#include <deque>
#include <functional>
#include <utility>

namespace lemon {
struct LEMON_PUBLIC event_args
{
};
class LEMON_PUBLIC event
{
  public:
    using observer_t = std::function<void(event_args*)>;

  public:
    void notify(event_args* args);
    void register_observer(const observer_t& observer, string_id name);
    void unregister_observer(string_id name);

  private:
    std::deque<std::pair<string_id, observer_t>> observers;
};
} // namespace lemon
