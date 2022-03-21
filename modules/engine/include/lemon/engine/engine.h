#pragma once

#include <lemon/assets/asset_storage.h>
#include <lemon/audio/sound_context.h>
#include <lemon/core/time/clock.h>
#include <lemon/platform/input.h>
#include <lemon/platform/window.h>
#include <lemon/rendering/rendering_context.h>
#include <lemon/scene/scene_manager.h>
#include <lemon/scripting/py_engine.h>
#include <lemon/threading/scheduler.h>

#include "py_bindings/py_core.h"

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

    owned<py_engine> _scriptingEngine;
    owned<scheduler> _scheduler;
    //    owned<event_handler> _events;
    owned<window> _window;
    owned<rendering_context> _renderingContext;
    owned<sound_context> _soundContext;
    owned<asset_storage> _resources;
    owned<input> _input;
    owned<clock> _clock;
    owned<scene_manager> _sceneManager;
};
}