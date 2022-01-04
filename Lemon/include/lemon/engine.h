#pragma once

#include "game/scene_manager.h"
#include "renderer/rendering_context.h"
#include "time/clock.h"
#include "window/window_base.h"
#include <memory>

namespace lemon {
class engine
{
  public:
    engine(int argc, char** argv);
    virtual ~engine();
    // void run();
    void initialize();
    bool update();

  protected:
    ptr<window_base> _window;
    ptr<rendering_context> _context;
    ptr<clock> _clock;
    ptr<scene_manager> _sceneManager;
};
}