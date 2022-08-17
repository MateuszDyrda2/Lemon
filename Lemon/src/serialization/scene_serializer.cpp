#include <serialization/scene_serializer.h>

#include <serialization/component_serializer.h>

#include <core/hash_string.h>
#include <world/components/entity_components.h>
#include <world/components/transform_components.h>

#include <fstream>
#include <vector>

namespace lemon {
using namespace rapidjson;
using namespace std;

struct output_archive
{
    StringBuffer s;
    Writer<StringBuffer> writer;
    int started;

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

    void operator()(entity ent)
    {
        writer.Uint(static_cast<underlying_type_t<entity>>(ent));
    }

    void operator()(underlying_type_t<entity> size)
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
    inline void operator()(entity ent, const T& component)
    {
        writer.Uint(static_cast<underlying_type_t<entity>>(ent));
        serialize_component<T>(component, writer);
    }
};

struct input_archive
{
    Document document;
    Value::ConstValueIterator entityIterator;
    Value::ConstValueIterator componentIterator;
    int started;

    input_archive(vector<char>& buffer):
        started{ 0 }
    {
        document.ParseInsitu(buffer.data());
        const Value& a = document["entities"];
        entityIterator = a.Begin();
    }

    void operator()(entity& ent)
    {
        ent = entity((entityIterator++)->GetUint());
    }

    void operator()(underlying_type_t<entity>& size)
    {
        if(started == 0)
        {
            size    = (entityIterator++)->GetUint();
            started = 1;
        }
        else if(started == 1)
        {
            const auto& a     = document["components"];
            entityIterator    = a.Begin();
            componentIterator = (entityIterator++)->Begin();
            size              = (componentIterator++)->GetUint();
            started           = 2;
        }
        else
        {
            componentIterator = (entityIterator++)->Begin();
            size              = (componentIterator++)->GetUint();
        }
    }
    template<class T>
    inline void operator()(entity& ent, T& component)
    {
        auto val                       = (componentIterator++)->GetUint();
        ent                            = entity(val);
        Value::ConstMemberIterator itr = (componentIterator++)->MemberBegin();
        deserialize_component<T>(component, itr);
    }
};

void scene_serializer::serialize(scene* scene, const std::string& path)
{
    entt::basic_snapshot snapshot(scene->get_registry());
    output_archive output;
    output.start();
    snapshot
        .entities(output)
        .component<transform, main_camera_t, camera, model>(output);

    output.end();

    ofstream f;
    f.open(path, ios::binary | ios::out);
    f.write(output.s.GetString(), output.s.GetSize());
    f.close();
}

registry&& scene_serializer::deserialize(const std::string& path)
{
    ifstream file;
    file.open(path, ios::binary | ios::in | ios::ate);
    std::size_t size = file.tellg();
    file.seekg(file.beg, 0);
    vector<char> buffer(size);
    file.read(buffer.data(), size);
    buffer.push_back('\0');
    input_archive input(buffer);
    registry reg;
    entt::basic_snapshot_loader loader(reg);
    loader
        .entities(input)
        .component<transform, main_camera_t, camera, model>(input)
        .orphans();

    return std::move(reg);
}
}