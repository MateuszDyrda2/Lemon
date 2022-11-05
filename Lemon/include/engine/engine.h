#pragma once

#include <assets/asset.h>
#include <events/events.h>
#include <platform/input.h>
#include <platform/window.h>
#include <rendering/rendering_context.h>
#include <scripting/scripting_engine.h>
#include <threading/scheduler.h>
#include <world/scene_manager.h>

namespace lemon {
class LEMON_API engine
{
  public:
    engine(int argc, char** argv, const std::string& assetPath);
    virtual ~engine();
    int run();

  protected:
    event_queue _eventQueue;
    message_bus _messageBus;
    window _window;
    input _input;
    rendering_context _renderingContext;
    scripting_engine _scriptingEngine;
    asset_storage _assertStorage;
    scheduler _scheduler;
    scene_manager _sceneManager;
};
} // namespace lemon

#define GAME_DECL(_GAME_NAME)                  \
    int main(int argc, char** argv)            \
    {                                          \
        auto eng = new _GAME_NAME(argc, argv); \
        auto ret = eng->run();                 \
        delete eng;                            \
        return ret;                            \
    }
