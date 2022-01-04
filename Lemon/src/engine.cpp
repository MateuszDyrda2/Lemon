#include <lemon/engine.h>

#include <lemon/event_system/event_handler.h>
#include <lemon/input/input.h>
#include <lemon/resources/resource_manager.h>
#include <lemon/service/services.h>

namespace lemon {
engine::engine(int, char**)
{
    LOG_MESSAGE("Engine created");
}
engine::~engine()
{
    services::cleanup();
    LOG_MESSAGE("Engine destroyed");
}
void engine::initialize()
{
    _context      = services::get<rendering_context>();
    _sceneManager = services::get<scene_manager>();
    _clock        = services::get<clock>();
    _window       = services::get<window_base>();
}
bool engine::update()
{
    _clock->update();
    _sceneManager->update();
    return !_window->end_frame();
}
}