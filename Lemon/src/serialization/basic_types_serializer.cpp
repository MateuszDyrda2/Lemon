#include <serialization/basic_types_serializer.h>

namespace lemon {
void serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int(value);
}
void serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint(value);
}
void serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int64(value);
}
void serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint64(value);
}
void serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Bool(value);
}
void serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Double(f64(value));
}
void serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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
    serialize(value[0], writer);
    serialize(value[1], writer);
    serialize(value[2], writer);
    serialize(value[3], writer);
    writer.EndArray();
}
void serialize(entity value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(entt::to_entity(value), writer);
}
void serialize(const color& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(value.rgba, writer);
}

void deserialize(i32& value, const rapidjson::Value& iter)
{
    value = iter.GetInt();
}
void deserialize(u32& value, const rapidjson::Value& iter)
{
    value = iter.GetUint();
}
void deserialize(i64& value, const rapidjson::Value& iter)
{
    value = iter.GetInt64();
}
void deserialize(u64& value, const rapidjson::Value& iter)
{
    value = iter.GetUint64();
}
void deserialize(bool& value, const rapidjson::Value& iter)
{
    value = iter.GetBool();
}
void deserialize(f32& value, const rapidjson::Value& iter)
{
    value = f32(iter.GetDouble());
}
void deserialize(f64& value, const rapidjson::Value& iter)
{
    value = f32(iter.GetDouble());
}
void deserialize(std::string& value, const rapidjson::Value& iter)
{
    value = iter.GetString();
}
void deserialize(vec2& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
              a[1].GetDouble() };
}
void deserialize(vec3& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
              a[1].GetDouble(),
              a[2].GetDouble() };
}
void deserialize(vec4& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
              a[1].GetDouble(),
              a[2].GetDouble(),
              a[3].GetDouble() };
}
void deserialize(mat4& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    auto b = a[0].GetArray();
    auto c = a[1].GetArray();
    auto d = a[2].GetArray();
    auto e = a[3].GetArray();
    value  = {
        {
            b[0].GetDouble(),
            b[1].GetDouble(),
            b[2].GetDouble(),
            b[3].GetDouble(),
        },
        {
            c[0].GetDouble(),
            c[1].GetDouble(),
            c[2].GetDouble(),
            c[3].GetDouble(),
        },
        {
            d[0].GetDouble(),
            d[1].GetDouble(),
            d[2].GetDouble(),
            d[3].GetDouble(),
        },
        {
            e[0].GetDouble(),
            e[1].GetDouble(),
            e[2].GetDouble(),
            e[3].GetDouble(),
        },
    };
}
void deserialize(entity& value, const rapidjson::Value& iter)
{
    value = entity(iter.GetUint());
}
void deserialize(color& value, const rapidjson::Value& iter)
{
    deserialize(value.rgba, iter);
}
}