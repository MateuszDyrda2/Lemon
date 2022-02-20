#include <lemon/game.h>

#include <lemon/engine.h>
#include <lemon/game/scene_manager.h>
#include <lemon/input/input.h>
#include <lemon/renderer/rendering_context.h>
#include <lemon/time/clock.h>
#include <lemon/window/window_base.h>

namespace lemon {
ptr<engine> game::eng = nullptr;
void game::start_game(ptr<engine> eng)
{
    game::eng = eng;
}
auto game::get_main_window() -> ptr<window_base>
{
    return eng->_window.get();
}
auto game::get_game_clock() -> ptr<clock>
{
    return eng->_clock.get();
}
auto game::get_scene_manager() -> ptr<scene_manager>
{
    return eng->_sceneManager.get();
}
auto game::get_input_handler() -> ptr<input>
{
    return eng->_input.get();
}

} // namespace lemon
