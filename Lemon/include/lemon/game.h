#pragma once

#include <lemon/core/basic_types.h>

namespace lemon {
class engine;
class window_base;
class clock;
class scene_manager;
class rendering_context;
class input;
class game
{
  public:
    static void start_game(ptr<engine> eng);
    static [[nodiscard]] auto get_main_window() -> ptr<window_base>;
    static [[nodiscard]] auto get_game_clock() -> ptr<clock>;
    static [[nodiscard]] auto get_scene_manager() -> ptr<scene_manager>;
    static [[nodiscard]] auto get_rendering_context() -> ptr<rendering_context>;
    static [[nodiscard]] auto get_input_handler() -> ptr<input>;

  private:
    static ptr<engine> eng;
};
} // namespace lemon
