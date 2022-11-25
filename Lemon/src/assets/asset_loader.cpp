#include "core/logger.h"
#include "core/utils.h"
#include <assets/asset_loader.h>

#include <rapidjson/document.h>

#include <fstream>

namespace lemon {
using namespace std;
using namespace rapidjson;
asset_loader::asset_loader(const string& path, scripting_engine& _scriptingEngine):
    path(path + "/assets.json"), _scriptingEngine(_scriptingEngine)
{
    ifstream stream(this->path, ios::binary | ios::ate);
    if (!stream)
    {
        logger::fatal("Failed to open asset file: {}", this->path);
        return;
    }
    std::size_t nBytes = stream.tellg();
    stream.seekg(0, stream.beg);
    vector<char> buff;
    buff.resize(nBytes + 1ULL);
    stream.read(buff.data(), nBytes);
    stream.close();

    buff.push_back('\0');
    Document document;
    document.ParseInsitu(buff.data());

    lemon_assert(document.IsObject());
    lemon_assert(document.HasMember("textures"));
    lemon_assert(document.HasMember("sounds"));
    lemon_assert(document.HasMember("shaders"));
    lemon_assert(document.HasMember("scripts"));
    lemon_assert(document.HasMember("animations"));

    auto&& textures   = document["textures"].GetArray();
    auto&& sounds     = document["sounds"].GetArray();
    auto&& shaders    = document["shaders"].GetArray();
    auto&& scripts    = document["scripts"].GetArray();
    auto&& animations = document["animations"].GetArray();

    run_for_each([this](auto& vv) {
        for (auto&& v : vv)
        {
            auto&& t                                   = v.GetObject();
            auto name                                  = t["name"].GetString();
            auto path                                  = t["path"].GetString();
            resourcePaths[hashstr::runtime_hash(name)] = path;
        }
    },
                 textures, sounds, shaders, scripts, animations);

    logger::info("Assets loaded");
}
asset_loader::~asset_loader()
{
}
bool asset_loader::resource_exists(hashstr nameid) const noexcept
{
    return resourcePaths.contains(nameid);
}
std::vector<char> asset_loader::load_from_file(const string& path)
{
    vector<char> buff;
    ifstream stream(path, ios::binary | ios::ate);
    if (!stream)
    {
        logger::error("Failed to load file: {}", path);
        return buff;
    }

    std::size_t nBytes = stream.tellg();
    stream.seekg(0, stream.beg);
    buff.reserve(nBytes + 1ULL);
    stream.read(buff.data(), nBytes);
    stream.close();
    return buff;
}
} // namespace lemon
