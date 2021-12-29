#include <river/engine.h>

#include <river/core/logger.h>
#include <river/event_system/event_handler.h>

namespace river {
engine::engine(int argc, char** argv)
{
    LOG_MESSAGE("Engine created");

    _window       = create_owned<window>("river", 1080, 720);
    _renderer     = create_owned<renderer>(_window.get());
    _sceneManager = create_owned<scene_manager>();
    _clock        = create_owned<clock>();
    (void)argc;
    (void)argv;
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
        _renderer->render(_sceneManager->get_current_scene());
        _window->end_frame();
    }
}
}