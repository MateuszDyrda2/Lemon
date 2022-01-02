#pragma once

#include "game/scene_manager.h"
#include "renderer/rendering_context.h"
#include "time/clock.h"
#include "window/window.h"
#include <memory>

namespace river {
class engine
{
  public:
    engine(int argc, char** argv);
    virtual ~engine();
    void run();

  protected:
    owned<window> _window;
    owned<rendering_context> _context;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
};
}