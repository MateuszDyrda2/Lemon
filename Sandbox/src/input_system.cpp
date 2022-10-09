#include "core/logger.h"
#include "input_system.h"
#include "platform/input.h"
#include "platform/keycodes.h"
#include <world/scene.h>

using namespace lemon;
input_system::input_system(scene& _scene, input& _input):
    _registry(_scene.get_registry()), _input(_input)
{
    _input.add_axis({ keycode::a, keycode::d }, "Horizontal"_hs);
    _input.add_axis({ keycode::s, keycode::w }, "Vertical"_hs);
}
input_system::~input_system()
{
}
