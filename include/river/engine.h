#pragma once

#include "game/scene_manager.h"
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
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
};
}