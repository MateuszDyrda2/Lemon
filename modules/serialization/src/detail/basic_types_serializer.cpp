#include <lemon/serialization/detail/basic_types_serializer.h>

namespace lemon {
namespace internal {
using namespace std;
using namespace rapidjson;
void serialize(int value, Writer<StringBuffer>& writer)
{
    writer.Int(value);
}
void serialize(u32 value, Writer<StringBuffer>& writer)
{
    writer.Uint(value);
}
void serialize(u64 value, Writer<StringBuffer>& writer)
{
    writer.Uint64(value);
}
void serialize(i64 value, Writer<StringBuffer>& writer)
{
    writer.Int64(value);
}
void serialize(bool value, Writer<StringBuffer>& writer)
{
    writer.Bool(value);
}
void serialize(f64 value, Writer<StringBuffer>& writer)
{
    writer.Double(value);
}
void serialize(f32 value, Writer<StringBuffer>& writer)
{
    writer.Double(value);
}
void serialize(const string& value, Writer<StringBuffer>& writer)
{
    writer.String(value.c_str(), value.length());
}
void serialize(const vec2& value, Writer<StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    writer.EndArray();
}
void serialize(const vec3& value, Writer<StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    writer.EndArray();
}
void serialize(const vec4& value, Writer<StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    serialize(value.w, writer);
    writer.EndArray();
}
void serialize(const mat4& value, Writer<StringBuffer>& writer)
{
    writer.StartArray();
    for(size_type i = 0; i < 4; ++i)
    {
        serialize(value[i], writer);
    }
    writer.EndArray();
}
void serialize(const string_id& value, Writer<StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.get_id(), writer);
    serialize(string(value.get_string()), writer);
    writer.EndArray();
}
void serialize(const entity_handle& value, Writer<StringBuffer>& writer)
{
    serialize((u32)value, writer);
}
void serialize(const color& col, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(col.rgba, writer);
}
void deserialize(i32& value, const Value& iter)
{
    value = iter.GetInt();
}
void deserialize(i64& value, const Value& iter)
{
    value = iter.GetInt64();
}
void deserialize(u32& value, const Value& iter)
{
    value = iter.GetUint();
}
void deserialize(u64& value, const Value& iter)
{
    value = iter.GetUint64();
}
void deserialize(f32& value, const Value& iter)
{
    value = iter.GetDouble();
}
void deserialize(f64& value, const Value& iter)
{
    value = iter.GetDouble();
}
void deserialize(string& value, const Value& iter)
{
    value = string(iter.GetString());
}
void deserialize(vec2& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(), a[1].GetDouble() };
}
void deserialize(vec3& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble() };
}
void deserialize(vec4& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble(), a[3].GetDouble() };
}
void deserialize(mat4& value, const Value& iter)
{
    auto a = iter.GetArray();
    auto b = a[0].GetArray();
    auto c = a[1].GetArray();
    auto d = a[2].GetArray();
    auto e = a[3].GetArray();

    value = {
        { b[0].GetDouble(), b[1].GetDouble(), b[2].GetDouble(), b[3].GetDouble() },
        { c[0].GetDouble(), c[1].GetDouble(), c[2].GetDouble(), c[3].GetDouble() },
        { d[0].GetDouble(), d[1].GetDouble(), d[2].GetDouble(), d[3].GetDouble() },
        { e[0].GetDouble(), e[1].GetDouble(), e[2].GetDouble(), e[3].GetDouble() }
    };
}
void deserialize(string_id& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = string_id(a[0].GetUint(), string(a[1].GetString()));
}
void deserialize(entity_handle& value, const Value& iter)
{
    value = entity_handle(iter.GetUint());
}
void deserialize(color& value, const rapidjson::Value& iter)
{
    vec4 rgba;
    deserialize(rgba, iter);
    value = color(rgba);
}
}
}