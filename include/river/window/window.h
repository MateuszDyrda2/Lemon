#pragma once

#include "window_base.h"

#include <river/event_system/event_handler.h>
#include <river/service/services.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>

namespace river {
class window : public window_base
{
  public:
    window(const std::string& name, size_type width, size_type height);
    ~window();

    bool end_frame() override;

    size_type get_width() const override { return _width; }
    size_type get_height() const override { return _height; }

    void* get_handle() override { return (void*)_handle; }

  private:
    GLFWwindow* _handle;
    std::string _name;
    size_type _width, _height;
    unsubscriber<int, int> windowResize;
    ptr<event_handler> eventHandler;

    void setup_callbacks();
};
}