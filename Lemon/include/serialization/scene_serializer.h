#pragma once

#include <core/defines.h>
#include <string>
#include <world/scene.h>

#include <serialization/component_serializer.h>
#include <serialization/system_serializer.h>

#include <core/hash_string.h>
#include <world/components/entity_components.h>
#include <world/components/transform_components.h>

#include <fstream>
#include <vector>

namespace lemon {
template<class... Args>
struct component_list_t
{ };

class LEMON_API scene_serializer
{
  public:
    template<class... Args>
    static void serialize(scene* s, const std::string& path, component_list_t<Args...>);
    template<class... Args>
    static registry&& deserialize(const std::string& path, component_list_t<Args...>);
};

struct LEMON_API output_archive
{
    scene* sc;
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer;
    int started;

    output_archive(scene* sc);
    void start();
    void end();
    void operator()(entity ent);
    void operator()(std::underlying_type_t<entity> size);
    template<class T>
    inline void operator()(entity ent, const T& component)
    {
        writer.Uint(static_cast<std::underlying_type_t<entity>>(ent));
        serialize_component<T>(component, writer);
    }
};

struct LEMON_API input_archive
{
    rapidjson::Document document;
    rapidjson::Value::ConstValueIterator entityIterator;
    rapidjson::Value::ConstValueIterator componentIterator;
    int started;

    input_archive(std::vector<char>& buffer);
    void operator()(entity& ent);
    void operator()(std::underlying_type_t<entity>& size);
    template<class T>
    inline void operator()(entity& ent, T& component)
    {
        auto val                                  = (componentIterator++)->GetUint();
        ent                                       = entity(val);
        rapidjson::Value::ConstMemberIterator itr = (componentIterator++)->MemberBegin();
        deserialize_component<T>(component, itr);
    }
};

template<class... Args>
void scene_serializer::serialize(scene* scene, const std::string& path, component_list_t<Args...>)
{
    entt::basic_snapshot snapshot(scene->get_registry());
    output_archive output(scene);
    output.start();
    snapshot
        .entities(output)
        .component<Args...>(output);
    output.end();

    std::ofstream f;
    f.open(path, std::ios::binary | std::ios::out);
    f.write(output.s.GetString(), output.s.GetSize());
    f.close();
}

template<class... Args>
registry&& scene_serializer::deserialize(const std::string& path, component_list_t<Args...>)
{
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in | std::ios::ate);
    std::size_t size = file.tellg();
    file.seekg(file.beg, 0);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);
    file.close();

    buffer.push_back('\0');
    input_archive input(buffer);
    registry reg;
    entt::basic_snapshot_loader loader(reg);
    loader
        .entities(input)
        .component<Args...>(input)
        .orphans();

    return std::move(reg);
}
}