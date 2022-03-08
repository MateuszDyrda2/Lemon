#pragma once

#include <lemon/assets/asset.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>
#include <lemon/core/string_id.h>
#include <lemon/scene/entity.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace lemon {
namespace internal {
void serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
template<class T>
inline void serialize(const std::vector<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    for(const auto& elem : value)
    {
        serialize(elem, writer);
    }
    writer.EndArray();
}
void serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const string_id& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void serialize(const entity_handle& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
template<class T>
inline void serialize(const asset<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(value.get_id(), writer);
}

void deserialize(i32& value, const rapidjson::Value& iter);
void deserialize(i64& value, const rapidjson::Value& iter);
void deserialize(u32& value, const rapidjson::Value& iter);
void deserialize(u64& value, const rapidjson::Value& iter);
void deserialize(f32& value, const rapidjson::Value& iter);
void deserialize(f64& value, const rapidjson::Value& iter);
void deserialize(std::string& value, const rapidjson::Value& iter);
void deserialize(vec2& value, const rapidjson::Value& iter);
void deserialize(vec3& value, const rapidjson::Value& iter);
void deserialize(vec4& value, const rapidjson::Value& iter);
void deserialize(mat4& value, const rapidjson::Value& iter);
void deserialize(string_id& value, const rapidjson::Value& iter);
void deserialize(entity_handle& value, const rapidjson::Value& iter);
template<class T>
inline void deserialize(std::vector<T>& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    for(auto i = a.Begin(); i != a.End(); ++i)
    {
        T element;
        deserialize(element, i);
        value.push_back(element);
    }
}
template<class T>
inline void deserialize(asset<T>& value, const rapidjson::Value& iter)
{
    string_id value_id;
    deserialize(value_id, iter);
    value = asset<T>(value_id);
}
}
} // namespace lemon
