#pragma once

#include "editor_window.h"
#include <river/engine.h>
#include <river/event_system/event_handler.h>
#include <river/renderer/framebuffer.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <ImGui/imgui_internal.h>

using namespace river;
class editor_engine : public engine
{
  public:
    editor_engine(int argc, char** argv);
    ~editor_engine();
    void initialize();
    friend class application;
};
class application
{
  public:
    application();
    ~application();
    void loop();

  private:
    std::unique_ptr<editor_engine> eng;
    editor_window* _window;
    std::unique_ptr<framebuffer> frameBuffer;
    event_handler* handler;
    ImVec2 oldViewport;
};