#include <world/scene_manager.h>

#include <filesystem>
#include <fstream>
#include <rapidjson/document.h>

#include <serialization/serializer.h>

namespace lemon {
using namespace rapidjson;
scene_manager::scene_manager(asset_storage& _assetStorage,
                             scheduler& _scheduler, event_queue& _eventQueue,
                             window& _window, input& _input, message_bus& _messageBus,
                             const std::string& settingsPath):
    _assetStorage(_assetStorage),
    _scheduler(_scheduler),
    _eventQueue(_eventQueue),
    _window(_window),
    _input(_input),
    _messageBus(_messageBus)
{
    std::ifstream stream(settingsPath, std::ios::binary | std::ios::ate);
    if (!stream)
    {
        logger::fatal("Failed to open project file: {}", settingsPath);
        return;
    }

    std::size_t nBytes = stream.tellg();
    stream.seekg(0, stream.beg);
    std::vector<char> buff;
    buff.resize(nBytes + 1ULL);
    stream.read(buff.data(), nBytes);
    stream.close();

    buff.push_back('\0');
    Document document;
    document.ParseInsitu(buff.data());

    auto&& scenes     = document["scenes"].GetArray();
    auto&& scenePath  = document["scene_path"].GetString();
    auto sceneAbsPath = std::filesystem::path(settingsPath).remove_filename().string()
                        + "/" + scenePath + "/";

    for (auto&& s : scenes)
    {
        auto str  = std::string(s.GetString());
        auto path = sceneAbsPath + str + ".json";
        sceneArray.emplace(str, path);
    }
}

scene_manager::~scene_manager()
{ }

void scene_manager::create_scene(hashstr sceneid)
{
    scenes.emplace(
        sceneid,
        new scene(sceneid, _assetStorage, _scheduler, _eventQueue, _window, _input, _messageBus));
}

scene& scene_manager::create_scene_defined(const std::string& sceneName, serializer& _serializer)
{
    auto id = hashstr::runtime_hash(sceneName.c_str());
    create_scene(id);
    auto& _scene = load_scene(id);
    _serializer.register_all(_scene.get_registry());
    const auto& scenePath = sceneArray[sceneName];
    _serializer.deserialize_scene(_scene, scenePath.c_str());
    return _scene;
}

scene& scene_manager::load_scene(hashstr sceneid)
{
    if (currentScene)
    {
        //        currentScene->on_unload();
    }
    currentScene = scenes.at(sceneid).get();
    //   currentScene->on_load();
    return *currentScene;
}

scene& scene_manager::get_current_scene()
{
    lemon_assert(currentScene);
    return *currentScene;
}

const scene& scene_manager::get_current_scene() const
{
    lemon_assert(currentScene);
    return *currentScene;
}

scene& scene_manager::get_scene(hashstr sceneid)
{
    return *scenes.at(sceneid);
}

const scene& scene_manager::get_scene(hashstr sceneid) const
{
    return *scenes.at(sceneid);
}

bool scene_manager::is_current(hashstr sceneid) const
{
    lemon_assert(currentScene);
    return currentScene->get_nameid() == sceneid;
}
}
