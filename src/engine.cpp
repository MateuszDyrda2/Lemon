#include <river/engine.h>

#include <river/core/logger.h>
#include <river/event_system/event_handler.h>
#include <river/input/input.h>
#include <river/resources/resource_manager.h>
#include <river/service/services.h>

namespace river {
engine::engine(int, char**)
{
    LOG_MESSAGE("Engine created");

    services::provide(create_owned<event_handler>());
    services::provide(create_owned<resource_manager>());
    _window = create_owned<window>("river", 1080, 720);
    services::provide(create_owned<input>(_window.get()));
    _context      = create_owned<rendering_context>();
    _sceneManager = create_owned<scene_manager>(_context.get());
    _clock        = create_owned<clock>();
}
engine::~engine()
{
    LOG_MESSAGE("Engine destroyed");
}
void engine::run()
{
    while(!_window->should_close())
    {
        _clock->update();
        _sceneManager->update(_clock->delta_time());
        _window->end_frame();
    }
}
}