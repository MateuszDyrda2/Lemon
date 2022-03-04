#pragma once

#include "scene/scene_manager.h"
#include <lemon/assets/asset_storage.h>
#include <lemon/core/time/clock.h>
#include <lemon/events/event_handler.h>
#include <lemon/threading/scheduler.h>
#include <lemon/window/input.h>
#include <lemon/window/window_base.h>

#include <memory>

namespace lemon {

class LEMON_PUBLIC engine
{
  public:
    engine(const std::string& workingDirectory, int argc, char** argv);
    virtual ~engine();
    // void run();
    void initialize();
    bool update();
    void drop();

  protected:
    friend class game;

    owned<scheduler> _scheduler;
    owned<event_handler> _events;
    owned<window_base> _window;
    owned<asset_storage> _resources;
    owned<input> _input;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
};
}