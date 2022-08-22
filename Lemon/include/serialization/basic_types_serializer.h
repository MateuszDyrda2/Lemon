#pragma once

#include <assets/asset.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>
#include <core/math/color.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <world/scene.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace lemon {
void LEMON_API serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(entity value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
void LEMON_API serialize(const color& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
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
template<class T>
inline void serialize(const asset<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(value.get_id(), writer);
}

void LEMON_API deserialize(i32& value, const rapidjson::Value& iter);
void LEMON_API deserialize(u32& value, const rapidjson::Value& iter);
void LEMON_API deserialize(i64& value, const rapidjson::Value& iter);
void LEMON_API deserialize(u64& value, const rapidjson::Value& iter);
void LEMON_API deserialize(bool& value, const rapidjson::Value& iter);
void LEMON_API deserialize(f32& value, const rapidjson::Value& iter);
void LEMON_API deserialize(f64& value, const rapidjson::Value& iter);
void LEMON_API deserialize(std::string& value, const rapidjson::Value& iter);
void LEMON_API deserialize(vec2& value, const rapidjson::Value& iter);
void LEMON_API deserialize(vec3& value, const rapidjson::Value& iter);
void LEMON_API deserialize(vec4& value, const rapidjson::Value& iter);
void LEMON_API deserialize(mat4& value, const rapidjson::Value& iter);
void LEMON_API deserialize(entity& value, const rapidjson::Value& iter);
void LEMON_API deserialize(color& value, const rapidjson::Value& iter);
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
    hash_str nameid;
    deserialize(nameid, iter);
    value = asset<T>(nameid);
}
}
