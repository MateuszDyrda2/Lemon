#include <lemon/assets/asset_loader.h>

#include <lemon/assets/file.h>
#include <lemon/game.h>

#include <rapidjson/document.h>

namespace lemon {
using namespace std;
asset_loader::asset_loader()
{
    using namespace rapidjson;

    file dataFile;
    dataFile.open(
        game::get_settings().workingDirectory + "/"
            + game::get_settings().assetPath + "assets.json",
        ios::binary | ios::in);
    vector<char> buff;
    dataFile.read(buff);
    dataFile.close();
    Document document;
    document.ParseInsitu(buff.data());

    for(auto a = document.Begin(); a != document.End(); ++a)
    {
        auto iter = a->MemberBegin();
        auto name = iter->value.GetString();
        ++iter;
        auto path                      = iter->value.GetString();
        resourcePaths[string_id(name)] = path;
    }
}
asset_loader::~asset_loader()
{
}
bool asset_loader::resource_exists(string_id name) const noexcept
{
    return resourcePaths.contains(name);
}
std::vector<byte> asset_loader::load_from_file(const string& path)
{
    file dataFile(path, ios::binary | ios::in);
    vector<byte> buff;
    dataFile.read(buff);
    dataFile.close();
    return buff;
}
} // namespace lemon
