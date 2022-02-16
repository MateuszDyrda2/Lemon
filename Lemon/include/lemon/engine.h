#pragma once

#include <memory>

namespace lemon {
class window_base;
class rendering_context;
class clock;
class scene_manager;
class input;
class asset_storage;
class event_handler;
class engine
{
  public:
    engine(int argc, char** argv);
    virtual ~engine();
    // void run();
    void initialize();
    bool update();

  protected:
    friend class game;
    owned<window_base> _window;
    owned<rendering_context> _context;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
    owned<input> _input;
    owned<scheduler> _scheduler;
    owned<asset_storage> _resources;
    owned<event_handler> _events;
};
}