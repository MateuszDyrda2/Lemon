#pragma once

#include "../glfw_devices.h"
#include "../key_codes.h"
#include "../window.h"

#include <lemon/core/string_id.h>
#include <lemon/scene/system.h>

#include <array>
#include <unordered_map>
#include <vector>

namespace lemon {
class scene;
struct gamepad
{
    size_type jid;
    std::array<key_state, GAMEPAD_KEY_COUNT> keyStates;
};
class LEMON_PUBLIC input_system : public system
{
  public:
    input_system(ptr<scene> s, ptr<window> w);
    ~input_system();
    void update(entity_registry& registry);
    void map2event(key::keycode kc, string_id eventName);

  private:
    std::array<key_state, KEY_COUNT> keyStates;
    std::array<string_id, KEY_COUNT> keyMapping;

    std::vector<input_device> inputDevices;
    glfw_devices devices;

  private:
    void gather_input();
};
} // namespace lemon
