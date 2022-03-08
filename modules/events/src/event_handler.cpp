#include <lemon/events/event_handler.h>

namespace lemon {
event_handler* event_handler::handler = nullptr;
event_handler::event_handler()
{
    LEMON_ASSERT(!handler);
    handler = this;
}
void event_handler::fire_event(string_id name, event_base* event)
{
    if(auto res = handler->reg.find(name); res != handler->reg.end())
    {
        for(auto&& fun : res->second)
        {
            fun(event);
        }
    }
    delete event;
}
void event_handler::fire_event(const std::string& name, event_base* event)
{
    if(auto res = handler->reg.find(string_id(name.c_str())); res != handler->reg.end())
    {
        for(auto&& fun : res->second)
        {
            fun(event);
        }
    }
    delete event;
}
void event_handler::subscribe(string_id name, const std::function<void(event_base*)>& fun)
{
    if(auto res = handler->reg.find(name); res != handler->reg.end())
    {
        res->second.push_back(fun);
    }
    else
    {

        handler->reg.insert(
            std::make_pair(
                name, std::list<std::function<void(event_base*)>>({ fun })));
    }
}
void event_handler::subscribe(const std::string& name, const std::function<void(event_base*)>& fun)
{
    string_id id(name.c_str());
    if(auto res = handler->reg.find(id); res != handler->reg.end())
    {
        res->second.push_back(fun);
    }
    else
    {

        handler->reg.insert(
            std::make_pair(
                id, std::list<std::function<void(event_base*)>>({ fun })));
    }
}
} // namespace lemon
