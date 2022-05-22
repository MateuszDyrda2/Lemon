#pragma once

#include <lemon/assets/asset_storage.h>
#include <lemon/audio/sound_context.h>
#include <lemon/core/time/game_clock.h>
#include <lemon/events/event_bus.h>
#include <lemon/platform/input.h>
#include <lemon/platform/window.h>
#include <lemon/rendering/rendering_context.h>
#include <lemon/scene/scene_manager.h>
#include <lemon/scripting/py_engine.h>
#include <lemon/threading/scheduler.h>

#include <lemon/core/service_registry.h>

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
    void run();
    void drop();

  protected:
    friend class game;
    service_registry globalRegistry;

    ptr<event_bus> eventBus;
    ptr<scheduler> sch;
    ptr<window> win;
    ptr<input> in;
    ptr<game_clock> clk;
    ptr<scene_manager> sceneManager;
};
}