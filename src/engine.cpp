#include <river/engine.h>

#include <river/core/logger.h>

namespace river {
engine::engine(int argc, char** argv)
{
    _window = new window("river", 1080, 720);
    LOG_MESSAGE("Engine created");
}
engine::~engine()
{
    delete _window;
    LOG_MESSAGE("Engine destroyed");
}
void engine::run()
{
    while(!_window->should_close())
    {
        _window->end_frame();
    }
}
}