#include <lemon/serialization/basic_types_serializer.h>

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
    serialize(value.get_cpp_string(), writer);
    writer.EndArray();
}
void serialize(const entity_handle& value, Writer<StringBuffer>& writer)
{
    serialize((u32)value, writer);
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
    value = iter.GetFloat();
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
    value  = { a[0].GetFloat(), a[1].GetFloat() };
}
void deserialize(vec3& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetFloat(), a[1].GetFloat(), a[2].GetFloat() };
}
void deserialize(vec4& value, const Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetFloat(), a[1].GetFloat(), a[2].GetFloat(), a[3].GetFloat() };
}
void deserialize(mat4& value, const Value& iter)
{
    auto a = iter.GetArray();
    auto b = a[0].GetArray();
    auto c = a[1].GetArray();
    auto d = a[2].GetArray();
    auto e = a[3].GetArray();

    value = {
        { b[0].GetFloat(), b[1].GetFloat(), b[2].GetFloat(), b[3].GetFloat() },
        { c[0].GetFloat(), c[1].GetFloat(), c[2].GetFloat(), c[3].GetFloat() },
        { d[0].GetFloat(), d[1].GetFloat(), d[2].GetFloat(), d[3].GetFloat() },
        { e[0].GetFloat(), e[1].GetFloat(), e[2].GetFloat(), e[3].GetFloat() }
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
}
}