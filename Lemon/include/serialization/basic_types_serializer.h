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
void serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(entity value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
void serialize(const color& value, rapidjson::Writer<rapidjson::StringBuffer>& iter);
template<class T>
inline void serialize(const std::vector<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& iter)
{
    iter.StartArray();
    for(const auto& elem : value)
    {
        serialize(elem, iter);
    }
    iter.EndArray();
}
template<class T>
inline void serialize(const asset<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& iter)
{
    serialize(value.get_id(), iter);
}

void deserialize(i32& value, const rapidjson::Value& iter);
void deserialize(u32& value, const rapidjson::Value& iter);
void deserialize(i64& value, const rapidjson::Value& iter);
void deserialize(u64& value, const rapidjson::Value& iter);
void deserialize(bool& value, const rapidjson::Value& iter);
void deserialize(f32& value, const rapidjson::Value& iter);
void deserialize(f64& value, const rapidjson::Value& iter);
void deserialize(std::string& value, const rapidjson::Value& iter);
void deserialize(vec2& value, const rapidjson::Value& iter);
void deserialize(vec3& value, const rapidjson::Value& iter);
void deserialize(vec4& value, const rapidjson::Value& iter);
void deserialize(mat4& value, const rapidjson::Value& iter);
void deserialize(entity& value, const rapidjson::Value& iter);
void deserialize(color& value, const rapidjson::Value& iter);
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
