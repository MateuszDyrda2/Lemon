#include <lemon/engine/engine.h>

#include <lemon/core/game.h>
#include <lemon/core/instrumentor.h>

#include <lemon/audio/sound_context.h>
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
    LEMON_PROFILE_FUNCTION();

    clk          = &globalRegistry.register_service<game_clock>();
    sch          = &globalRegistry.register_service<scheduler>(std::thread::hardware_concurrency() - 1);
    eventBus     = &globalRegistry.register_service<event_bus>();
    win          = &globalRegistry.register_service<window>(1920, 1080);
    in           = &globalRegistry.register_service<input>();
    sceneManager = &globalRegistry.register_service<scene_manager>();

    rendering_context::create();
    sound_context::drop();
}
bool engine::update()
{
    LEMON_PROFILE_FUNCTION();

    clk->update();
    in->update();
    sceneManager->update();
    return win->end_frame();
}
void engine::drop()
{
    LEMON_PROFILE_FUNCTION();

    sound_context::drop();
    rendering_context::drop();

    globalRegistry.unregister_service<scene_manager>();
    globalRegistry.unregister_service<input>();
    globalRegistry.unregister_service<window>();
    globalRegistry.unregister_service<event_bus>();
    globalRegistry.unregister_service<scheduler>();
    globalRegistry.unregister_service<game_clock>();
}
}
