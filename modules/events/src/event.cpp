#include <lemon/events/event.h>

#include <algorithm>

namespace lemon {
void event::notify(event_args* args)
{
    for(auto& o : observers)
    {
        o.second(args);
    }
}
void event::register_observer(const observer_t& observer, string_id name)
{
    observers.push_back({ name, observer });
}
void event::unregister_observer(string_id name)
{
    std::remove_if(
        observers.begin(), observers.end(),
        [name](std::pair<string_id, observer_t>& p) { return p.first == name; });
}
} // namespace lemon
