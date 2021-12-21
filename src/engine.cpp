#include <river/engine.h>

namespace river {
engine::engine(int argc, char** argv)
{
    _window = new window("river", 1080, 720);
}
engine::~engine()
{
    delete _window;
}
void engine::run()
{
    while(!_window->should_close())
    {
        _window->end_frame();
    }
}
}