#include <lemon/serialization/scene_serializer.h>

#include <lemon/assets/file.h>
#include <lemon/game.h>
#include <lemon/serialization/component_serializer.h>

#include <entt/entt.hpp>

namespace lemon {
using namespace rapidjson;
using namespace std;

struct output_archive
{
    output_archive():
        s(), writer(s), started{ 0 }
    { }
    void start()
    {
        writer.StartObject();
    }
    void end()
    {
        writer.EndArray();
        writer.EndArray();
        writer.EndObject();
    }
    void operator()(entt::entity entity)
    {
        writer.Uint(static_cast<underlying_type_t<entt::entity>>(entity));
    }
    void operator()(std::underlying_type_t<entt::entity> size)
    {
        if(!started)
        {
            writer.Key("entities");
            ++started;
        }
        else if(started == 1)
        {
            writer.EndArray();
            writer.Key("components");
            writer.StartArray();
            ++started;
        }
        else
        {
            writer.EndArray();
        }
        writer.StartArray();
        writer.Uint(size);
    }
    template<class T>
    inline void operator()(entt::entity entity, const T& component)
    {
        writer.Uint(static_cast<underlying_type_t<entt::entity>>(entity));
        serialize_component<T>(component, writer);
    }

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer;
    int started;
};
struct input_archive
{
    input_archive(vector<char>& buffer):
        started{ 0 }
    {
        document.ParseInsitu(buffer.data());
        const Value& a = document["entities"];
        enttIterator   = a.Begin();
    }
    void operator()(entt::entity& entity)
    {
        auto val = enttIterator->GetUint();
        entity   = entt::entity((enttIterator++)->GetUint());
    }
    void operator()(std::underlying_type_t<entt::entity>& size)
    {
        if(started == 0)
        {
            size    = (enttIterator++)->GetUint();
            started = 1;
        }
        else if(started == 1)
        {
            const auto& a     = document["components"];
            enttIterator      = a.Begin();
            componentIterator = enttIterator->Begin();
            size              = componentIterator->GetUint();
            started           = 2;
            ++enttIterator;
            ++componentIterator;
        }
        else
        {
            componentIterator = enttIterator->Begin();
            size              = componentIterator->GetUint();
            ++enttIterator;
            ++componentIterator;
        }
    }
    template<class T>
    inline void operator()(entt::entity& entity, T& component)
    {
        auto val                       = (componentIterator++)->GetUint();
        entity                         = entt::entity(val);
        Value::ConstMemberIterator itr = componentIterator->MemberBegin();
        deserialize_component<T>(component, itr);
        componentIterator++;
    }

    Document document;
    Value::ConstValueIterator enttIterator;
    Value::ConstValueIterator componentIterator;
    int started;
};
void scene_serializer::serialize(ptr<scene> scene)
{
    entt::basic_snapshot snapshot(scene->get_registry());
    output_archive output;
    output.start();
    snapshot
        .entities(output)
        .component<tag, transform, camera, sprite_renderer>(output);
    output.end();
    file f;

    f.open(
        game::get_settings().workingDirectory + "/"
            + game::get_settings().scenePath + scene->get_name() + ".json",
        ios::binary | ios::out);
    f.write(output.s.GetString(), output.s.GetSize());
    f.close();
}
ptr<scene> scene_serializer::deserialize(const std::string& name)
{
    file f;

    f.open(
        game::get_settings().workingDirectory + "/"
            + game::get_settings().scenePath + name + ".json",
        ios::binary | ios::in);
    vector<char> buffer;
    f.read(buffer);
    f.close();
    buffer.push_back('\0');
    input_archive input(buffer);
    entity_registry registry;
    entt::basic_snapshot_loader loader(registry);
    loader
        .entities(input)
        .component<tag, transform, camera, sprite_renderer>(input)
        .orphans();
    return new scene(string_id(name.c_str()), std::move(registry));
}
}