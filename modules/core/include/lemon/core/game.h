#pragma once

#include "basic_types.h"
#include "defines.h"

namespace lemon {
class engine;
class window_base;
class clock;
class scene_manager;
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
    static void start_game(const std::string& projFile);
    /** @return pointer to the game's main window */
    static auto get_main_window() -> ptr<window_base> { return _window; }
    /** @return pointer to the game's main clock */
    static auto get_game_clock() -> ptr<clock> { return _clock; }
    /** @return pointer to the game's scene manager */
    static auto get_scene_manager() -> ptr<scene_manager> { return _sceneManager; }
    static const settings& get_settings() { return gameSettings; }

    inline static void provide_window(ptr<window_base> window) { _window = window; }
    inline static void provide_clock(ptr<clock> clock) { _clock = clock; }
    inline static void provide_scene_manager(ptr<scene_manager> sceneManager) { _sceneManager = sceneManager; }

  private:
    static ptr<window_base> _window;
    static ptr<clock> _clock;
    static ptr<scene_manager> _sceneManager;

    static settings gameSettings;
};
} // namespace lemon
