#include <lemon/serialization/scene_serializer.h>

#include <lemon/assets/file.h>

namespace lemon {
using namespace rapidjson;
using namespace std;
scene_serializer::output_archive::output_archive():
    s(), writer(s), started{ 0 }
{ }
void scene_serializer::output_archive::start()
{
    writer.StartObject();
}
void scene_serializer::output_archive::end()
{
    writer.EndArray();
    writer.EndArray();
    writer.EndObject();
}
void scene_serializer::output_archive::operator()(entt::entity entity)
{
    writer.Uint(u32(entity));
}
void scene_serializer::output_archive::operator()(std::underlying_type_t<entt::entity> size)
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
void scene_serializer::output_archive::operator()(entt::entity entity, const T& component)
{
    writer.Uint(u32(entity));
    serialize_component<T>(component, writer);
}

struct input_archive
{
    void operator()(entt::entity& entity);
    void operator()(std::underlying_type_t<entt::entity>& size);
    template<class T>
    void operator()(entt::entity& entity, const T& component);
};
scene_serializer::scene_serializer()
{
}
scene_serializer::~scene_serializer()
{
}
ptr<scene_serializer> scene_serializer::serialize(ptr<scene> scene)
{
    entt::basic_snapshot snapshot(scene->get_registry());
    output.start();
    snapshot
        .entities(output)
        .component<tag, transform, camera, sprite_renderer>(output);
    output.end();
    return this;
}
ptr<scene_serializer> scene_serializer::save2file(const std::string& path)
{
    file f;
    f.open(path, ios::binary | ios::out);
    f.write(output.s.GetString(), output.s.GetSize());
    f.close();
    return this;
}
scene_deserializer::scene_deserializer()
{
}
scene_deserializer::~scene_deserializer()
{
}
ptr<scene_deserializer> scene_deserializer::read_file(const std::string& path)
{
    file f;
    f.open(path, ios::binary | ios::in);
    vector<char> buffer;
    f.read(buffer);
    f.close();
    input.document.Parse(buffer.data());
    return this;
}
ptr<scene_deserializer> scene_deserializer::deserialize(ptr<scene> scene)
{
    entt::basic_snapshot_loader loader(scene->get_registry());
    loader
        .entities(input)
        .component<tag, transform, camera, sprite_renderer>(input)
        .orphans();
    return this;
}
scene_deserializer::input_archive::input_archive():
    started{ 0 }
{
    const Value& a = document["entities"];
    enttIterator   = a.Begin();
    endIterator    = a.End();
}
void scene_deserializer::input_archive::operator()(entt::entity& entity)
{
    entity = entt::entity((enttIterator++)->GetUint());
}
void scene_deserializer::input_archive::operator()(std::underlying_type_t<entt::entity>& size)
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
void scene_deserializer::input_archive::operator()(entt::entity& entity, T& component)
{
    entity                         = entt::entity((componentIterator++)->GetUint());
    Value::ConstMemberIterator itr = componentIterator->MemberBegin();
    deserialize_component<T>(component, itr);
    componentIterator++;
}
}