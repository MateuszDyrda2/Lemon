#include <lemon/engine/engine.h>

#include <lemon/core/game.h>
#include <lemon/core/instrumentor.h>
#include <lemon/rendering/rendering_context.h>

namespace lemon {
engine::engine(const std::string& workingDirectory, int, char**)
{
    py_init_core();
    game::start_game(workingDirectory);
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
    LEMON_PROFILE_FUNCTION();
    _clock->update();
    _eventBus->flush();
    _sceneManager->update();
    return !_window->end_frame();
}
void engine::drop()
{
}
}
