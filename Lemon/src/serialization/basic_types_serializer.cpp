#include <serialization/basic_types_serializer.h>

namespace lemon {
void basic_types_serializer::serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int(value);
}

void basic_types_serializer::serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint(value);
}

void basic_types_serializer::serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Int64(value);
}

void basic_types_serializer::serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Uint64(value);
}

void basic_types_serializer::serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Bool(value);
}

void basic_types_serializer::serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Double(f64(value));
}

void basic_types_serializer::serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Double(value);
}

void basic_types_serializer::serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.String(value.c_str(), value.length());
}

void basic_types_serializer::serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    writer.EndArray();
}

void basic_types_serializer::serialize(const ivec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    writer.EndArray();
}

void basic_types_serializer::serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    writer.EndArray();
}

void basic_types_serializer::serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value.x, writer);
    serialize(value.y, writer);
    serialize(value.z, writer);
    serialize(value.w, writer);
    writer.EndArray();
}
void basic_types_serializer::serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    serialize(value[0], writer);
    serialize(value[1], writer);
    serialize(value[2], writer);
    serialize(value[3], writer);
    writer.EndArray();
}

void basic_types_serializer::serialize(entity_t value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(entt::to_entity(value), writer);
}

void basic_types_serializer::serialize(const color& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(value.rgba, writer);
}

void basic_types_serializer::deserialize(i32& value, const rapidjson::Value& iter)
{
    value = iter.GetInt();
}

void basic_types_serializer::deserialize(u32& value, const rapidjson::Value& iter)
{
    value = iter.GetUint();
}

void basic_types_serializer::deserialize(i64& value, const rapidjson::Value& iter)
{
    value = iter.GetInt64();
}

void basic_types_serializer::deserialize(u64& value, const rapidjson::Value& iter)
{
    value = iter.GetUint64();
}

void basic_types_serializer::deserialize(bool& value, const rapidjson::Value& iter)
{
    value = iter.GetBool();
}

void basic_types_serializer::deserialize(f32& value, const rapidjson::Value& iter)
{
    value = f32(iter.GetDouble());
}

void basic_types_serializer::deserialize(f64& value, const rapidjson::Value& iter)
{
    value = f32(iter.GetDouble());
}

void basic_types_serializer::deserialize(std::string& value, const rapidjson::Value& iter)
{
    value = iter.GetString();
}

void basic_types_serializer::deserialize(vec2& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
               a[1].GetDouble() };
}

void basic_types_serializer::deserialize(ivec2& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetInt(),
               a[1].GetInt() };
}

void basic_types_serializer::deserialize(vec3& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
               a[1].GetDouble(),
               a[2].GetDouble() };
}

void basic_types_serializer::deserialize(vec4& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    value  = { a[0].GetDouble(),
               a[1].GetDouble(),
               a[2].GetDouble(),
               a[3].GetDouble() };
}

void basic_types_serializer::deserialize(mat4& value, const rapidjson::Value& iter)
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
void basic_types_serializer::deserialize(entity_t& value, const rapidjson::Value& iter)
{
    value = entity_t(iter.GetUint());
}

void basic_types_serializer::deserialize(color& value, const rapidjson::Value& iter)
{
    deserialize(value.rgba, iter);
}
}
