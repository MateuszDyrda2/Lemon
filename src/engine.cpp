#include <river/engine.h>

#include <river/core/logger.h>

namespace river {
engine::engine(int argc, char** argv)
{
    LOG_MESSAGE("Engine created");
    _window   = std::make_unique<window>("river", 1080, 720);
    _renderer = std::make_unique<renderer>(_window.get());
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
        _renderer->render();

        _window->end_frame();
    }
}
}