#pragma once

#include "window_events.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/service.h>
#include <lemon/threading/scheduler.h>

#include <string>

namespace lemon {
class service_registry;
class LEMON_PUBLIC window : public listener, public service
{
  public:
    LEMON_REGISTER_SYSTEM(window);
    using native_handle_t = void*;

  public:
    window(service_registry& globalRegistry, size_type width, size_type height);
    ~window();
    void on_event(event* e) override;

    bool update();
    ivec2 get_size() const { return size; }

    native_handle_t get_handle() { return (void*)_handle; }

  private:
    event_bus& ebus;
    std::string _name;
    ivec2 size;
    native_handle_t _handle;

  private:
    static void setup_callbacks(native_handle_t handle);
};
}