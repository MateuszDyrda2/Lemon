#pragma once

#include "window_base.h"

#include <lemon/events/dispatcher.h>
#include <lemon/events/listener.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>

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
    listener<int, int> windowResize;
    dispatcher eventDispatcher;

    void setup_callbacks();
};
}