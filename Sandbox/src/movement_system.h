#pragma once

#include <world/system.h>

using namespace lemon;
class movement_system : public system
{
  public:
    SYSTEM(movement_system, scene, event_queue, input);
    ~movement_system();
    void onUpdate(event_args* e);

  private:
    input& _input;
    scene& _scene;
    event_queue::listener_handle horizontal, vertical;
    event_queue::listener_handle update;

    enum direction
    {
        NONE,
        LEFT,
        RIGHT,
        BACK,
        FORWARD,
        LEFT_BACK,
        RIGHT_BACK,
        LEFT_FORWARD,
        RIGHT_FORWARD,
    } currentDirection;

    static direction get_direction(const vec2& velocity);
};
