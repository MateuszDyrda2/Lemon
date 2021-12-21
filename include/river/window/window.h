#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace river {
class window
{
  public:
    window(std::string const& name, int width, int height);
    ~window();

    bool should_close();
    void end_frame();

  private:
    GLFWwindow* handle;
    int width, height;
};
}