#pragma once

#include "game/scene_manager.h"
#include "renderer/renderer.h"
#include "time/clock.h"
#include "window/window.h"
#include <memory>

namespace river {
class engine
{
  public:
    engine(int argc, char** argv);
    ~engine();
    void run();

  private:
    owned<window> _window;
    owned<renderer> _renderer;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
};
}