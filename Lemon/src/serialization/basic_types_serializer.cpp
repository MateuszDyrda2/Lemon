#include <lemon/serialization/basic_types_serializer.h>

namespace lemon {
namespace internal {
void serialize(int value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int(value);
}
void serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint(value);
}
void serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint64(value);
}
void serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int64(value);
}
void serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Bool(value);
}
void serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Double(value);
}
void serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Double(value);
}
void serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.String(value.c_str(), value.length());
}
void serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    writer.EndArray();
}
void serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    writer.EndArray();
}
void serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    serialize(value.w, writer);
    writer.EndArray();
}
void serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    for(size_type i = 0; i < 4; ++i)
    {
        serialize(value[i], writer);
    }
    writer.EndArray();
}
void serialize(const string_id& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.get_id(), writer);
    serialize(value.get_cpp_string(), writer);
    writer.EndArray();
}
void serialize(const entity_handle& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize((u32)value, writer);
}
}
}