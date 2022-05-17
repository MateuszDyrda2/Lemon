#pragma once

#include "glfw_devices.h"
#include "key_codes.h"
#include "window.h"

#include <lemon/core/service.h>

#include <array>
#include <unordered_map>
#include <vector>

namespace lemon {
class scene;
class service_registry;
struct gamepad
{
    size_type jid;
    std::array<key_state, GAMEPAD_KEY_COUNT> keyStates;
};
class LEMON_PUBLIC input : public service
{
  public:
    LEMON_REGISTER_SYSTEM(input);

  public:
    input(service_registry& globalRegistry);
    ~input();
    void update();
    void map2event(key::keycode kc, string_id eventName);

  private:
    window& win;
    std::array<key_state, KEY_COUNT> keyStates;
    std::array<string_id, KEY_COUNT> keyMapping;

    std::vector<input_device> inputDevices;
    glfw_devices devices;

  private:
    void gather_input();
};
} // namespace lemon
