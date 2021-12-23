#pragma once

#include <GLFW/glfw3.h>
#include <string>

namespace river {
class window
{
  public:
    window(const std::string& name, int width, int height);
    ~window();

    bool should_close();
    void end_frame();

  private:
    GLFWwindow* _handle;
    std::string _name;
    int _width, _height;
};
}