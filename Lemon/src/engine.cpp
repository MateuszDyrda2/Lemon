#include <lemon/engine.h>

#include <lemon/assets/asset_storage.h>
#include <lemon/events/event_handler.h>
#include <lemon/game.h>
#include <lemon/game/scene_manager.h>
#include <lemon/input/input.h>
#include <lemon/renderer/rendering_context.h>
#include <lemon/threads/scheduler.h>
#include <lemon/time/clock.h>
#include <lemon/window/window.h>

namespace lemon {
engine::engine(const std::string& workingDirectory, int, char**)
{
    game::start_game(this, workingDirectory);
    LOG_MESSAGE("Engine created");
}
engine::~engine()
{
    LOG_MESSAGE("Engine destroyed");
}
void engine::initialize()
{
}
bool engine::update()
{
    _clock->update();
    _sceneManager->update();
    return !_window->end_frame();
}
void drop()
{
}
}