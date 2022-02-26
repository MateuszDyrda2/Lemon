#pragma once

#include <lemon/core/basic_types.h> // for ptr<>
#include <lemon/engine.h>

namespace lemon {
/** Game class for accessing single instance components of the game engine */
class LEMON_PUBLIC game
{
  public:
    struct settings
    {
        std::string workingDirectory;
        std::string gameName;
        std::string assetPath;
        std::string scenePath;
        std::string startingScene;
    };

  public:
    /** @brief Provides the game class with pointer to the engine
     * @param eng engine pointer
     */
    static void start_game(ptr<engine> eng, const std::string& workingDirectory);
    /** @return pointer to the game's main window */
    static auto get_main_window() -> ptr<window_base>;
    /** @return pointer to the game's main clock */
    static auto get_game_clock() -> ptr<clock>;
    /** @return pointer to the game's scene manager */
    static auto get_scene_manager() -> ptr<scene_manager>;
    /** @return pointe to the game's input handler */
    static auto get_input_handler() -> ptr<input>;
    static const settings& get_settings() { return gameSettings; }

  private:
    static ptr<engine> eng; ///< pointer to the engine instance
    static settings gameSettings;
};
} // namespace lemon