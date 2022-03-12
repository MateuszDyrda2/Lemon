#include <lemon/core/game.h>

#include <lemon/core/file.h>

#include <rapidjson/document.h>
#include <vector>

namespace lemon {
using namespace std;
using namespace rapidjson;
game::settings game::gameSettings;
ptr<window> game::_window              = nullptr;
ptr<clock> game::_clock                = nullptr;
ptr<scene_manager> game::_sceneManager = nullptr;

game::settings
parse_settings(const std::string& projFile);

void game::start_game(const std::string& projFile)
{
    game::gameSettings = parse_settings(projFile);
}
game::settings parse_settings(const std::string& projFile)
{
    file file;
    file.open(projFile, ios::in | ios::binary);
    vector<char> buffer;
    file.read(buffer);
    file.close();

    Document document;
    document.ParseInsitu(buffer.data());
    auto iter = document.MemberBegin();
    game::settings settings;
    settings.workingDirectory = projFile.substr(0, projFile.find_last_of("\\/"));
    settings.gameName         = (iter++)->value.GetString();
    settings.assetPath        = (iter++)->value.GetString();
    settings.scenePath        = (iter++)->value.GetString();
    settings.startingScene    = iter->value.GetString();
    return settings;
}

} // namespace lemon
