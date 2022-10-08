#include "core/logger.h"
#include "input_system.h"
#include "platform/input.h"
#include "platform/keycodes.h"

using namespace lemon;
input_system::input_system(registry& _registry, event_queue& _eventQueue,
                           input& _input):
    _registry(_registry),
    _eventQueue(_eventQueue), _input(_input)
{
    _input.add_axis({ keycode::a, keycode::d }, "Horizontal"_hs);
    _input.add_axis({ keycode::s, keycode::w }, "Vertical"_hs);

    hor = _eventQueue
              .event("Horizontal"_hs)
              .subscribe([](event_args* e) {
                  auto ka = *static_cast<axis_event*>(e);
                  if(ka.value != 0)
                      logger::info("{}", ka.value > 0 ? "right" : "left");
              });

    vert = _eventQueue
               .event("Vertical"_hs)
               .subscribe([](event_args* e) {
                   auto ka = *static_cast<axis_event*>(e);
                   if(ka.value != 0)
                       logger::info("{}", ka.value > 0 ? "up" : "down");
               });
}
input_system::~input_system()
{
}
void input_system::update()
{
}
