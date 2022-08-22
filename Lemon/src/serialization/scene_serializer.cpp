#include <serialization/scene_serializer.h>

namespace lemon {
using namespace rapidjson;
using namespace std;

output_archive::output_archive(scene* sc):
    sc(sc), s(), writer(s), started{ 0 }
{ }

void output_archive::start()
{
    writer.StartObject();
    writer.Key("systems");
    writer.StartArray();
    serialize_systems(sc->get_systems(), writer);
    writer.EndArray();
}

void output_archive::end()
{
    writer.EndArray();
    writer.EndArray();
    writer.EndObject();
}

void output_archive::operator()(entity ent)
{
    writer.Uint(static_cast<underlying_type_t<entity>>(ent));
}

void output_archive::operator()(underlying_type_t<entity> size)
{
    if(!started)
    {
        writer.Key("entities");
        writer.StartArray();
        ++started;
    }
    else if(started == 1)
    {
        writer.EndArray();
        writer.EndArray();
        writer.Key("components");
        writer.StartArray();
        ++started;
    }
    else
    {
        writer.EndArray();
    }
    writer.Uint(size);
    writer.StartArray();
}

input_archive::input_archive(vector<char>& buffer):
    started{ 0 }
{
    document.ParseInsitu(buffer.data());
    const Value& a = document["entities"];
    entityIterator = a.Begin();
}

void input_archive::operator()(entity& ent)
{
    ent = entity((entityIterator++)->GetUint());
}

void input_archive::operator()(underlying_type_t<entity>& size)
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
}