/** @file engine.h
 * @brief File with a definition of an engine type
 */
#pragma once

#include "serialization/serializer.h"
#include <assets/asset.h>
#include <events/events.h>
#include <platform/input.h>
#include <platform/window.h>
#include <rendering/rendering_context.h>
#include <scripting/scripting_engine.h>
#include <threading/scheduler.h>
#include <world/scene_manager.h>

namespace lemon {
/** engine class */
class LEMON_API engine
{
  public:
    /** @brief Constructs an engine
     * @param argc number of arguments the program was started with
     * @param argv arguments the program was started with
     * @param assetPath path to a file containing all the assets
     */
    engine(int argc, char** argv, const std::string& assetPath, const std::string& settingsPath);
    virtual ~engine();
    /** @brief Start the engine
     * @return return code
     */
    int run();

  protected:
    event_queue _eventQueue;
    message_bus _messageBus;
    window _window;
    input _input;
    rendering_context _renderingContext;
    scripting_engine _scriptingEngine;
    asset_storage _assetStorage;
    scheduler _scheduler;
    scene_manager _sceneManager;
    serializer _serializer;
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
