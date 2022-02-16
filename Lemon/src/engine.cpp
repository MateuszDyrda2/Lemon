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
engine::engine(int, char**)
{
    game::start_game(this);
    LOG_MESSAGE("Engine created");
}
engine::~engine()
{
    LOG_MESSAGE("Engine destroyed");
}
void engine::initialize()
{
    _scheduler    = create_owned<scheduler>(std::thread::hardware_concurrency - 1);
    _events       = create_owned<event_handler>();
    _resources    = create_owned<asset_storage>("path");
    _clock        = create_owned<clock>();
    _window       = create_owned<window>("lemon", 1920, 1080);
    _context      = create_owned<rendering_context>();
    _input        = create_owned<input>(_window.get());
    _sceneManager = create_owned<scene_manager>(_context.get());
}
bool engine::update()
{
    _clock->update();
    _sceneManager->update();
    return !_window->end_frame();
}
}