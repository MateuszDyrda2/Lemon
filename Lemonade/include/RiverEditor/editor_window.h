#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/window/window_base.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class editor_window : public lemon::window_base
{
  public:
    editor_window(std::size_t width, std::size_t height);
    ~editor_window();

    bool end_frame() override;
    void* get_handle() override { return _handle; }

  private:
    GLFWwindow* _handle;
};