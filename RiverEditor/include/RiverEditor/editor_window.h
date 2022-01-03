#pragma once

#include <river/core/basic_types.h>
#include <river/window/window_base.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class editor_window : public river::window_base
{
  public:
    editor_window(std::size_t width, std::size_t height);
    ~editor_window();

    bool end_frame() override;
    void* get_handle() override { return _handle; }

  private:
    GLFWwindow* _handle;
};