#pragma once

#include "core/basic_types.h"
#include <memory>

namespace lemon {
class window_base;
class clock;
class scene_manager;
class input;

class scheduler;
class asset_storage;
class event_handler;

class engine
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

    owned<window_base> _window;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
    owned<input> _input;

    owned<scheduler> _scheduler;
    owned<asset_storage> _resources;
    owned<event_handler> _events;
};
}