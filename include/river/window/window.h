#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <river/core/basic_types.h>
#include <river/event_system/event_handler.h>
#include <string>

namespace river {
class window
{
  public:
    window(const std::string& name, size_type width, size_type height);
    ~window();

    bool should_close();
    void end_frame();

    size_type get_width() const { return _width; }
    size_type get_height() const { return _height; }

    void on_resize(size_type width, size_type height);

  private:
    GLFWwindow* _handle;
    std::string _name;
    size_type _width, _height;
    unsubscriber<size_type, size_type> windowResize;

    void setup_callbacks();
};
}