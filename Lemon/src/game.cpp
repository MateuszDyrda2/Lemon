#include <lemon/game.h>

#include <lemon/assets/file.h>
#include <lemon/engine.h>
#include <lemon/game/scene_manager.h>
#include <lemon/input/input.h>
#include <lemon/renderer/rendering_context.h>
#include <lemon/time/clock.h>
#include <lemon/window/window_base.h>

#include <rapidjson/document.h>
#include <vector>

namespace lemon {
using namespace std;
using namespace rapidjson;
ptr<engine> game::eng = nullptr;
game::settings game::gameSettings;

game::settings parse_settings(const std::string& workingDirectory);

void game::start_game(ptr<engine> eng, const std::string& workingDirectory)
{
    game::eng          = eng;
    game::gameSettings = parse_settings(workingDirectory);
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
game::settings parse_settings(const std::string& workingDirectory)
{
    file file;
    file.open(workingDirectory + "/lemon.json", ios::in | ios::binary);
    vector<char> buffer;
    file.read(buffer);
    file.close();

    Document document;
    document.ParseInsitu(buffer.data());
    auto iter = document.MemberBegin();
    game::settings settings;
    settings.workingDirectory = workingDirectory;
    settings.gameName         = (iter++)->value.GetString();
    settings.assetPath        = (iter++)->value.GetString();
    settings.scenePath        = (iter++)->value.GetString();
    settings.startingScene    = iter->value.GetString();
    return settings;
}

} // namespace lemon
