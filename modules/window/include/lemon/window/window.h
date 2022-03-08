#pragma once

#include "window_base.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>

#include <lemon/events/event_handler.h>

namespace lemon {
class LEMON_PUBLIC window : public window_base
{
  public:
    window(size_type width, size_type height);
    ~window();

    bool end_frame() override;
    void* get_handle() override { return (void*)_handle; }

  private:
    GLFWwindow* _handle;
    std::string _name;

    void setup_callbacks();
};
}