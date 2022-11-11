#pragma once

#include <assets/asset.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>
#include <core/math/color.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>
#include <type_traits>
#include <world/scene.h>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace lemon {
struct LEMON_API basic_types_serializer
{
    void serialize(i32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(u32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(i64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(u64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(bool value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(f32 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(f64 value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const std::string& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const vec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const ivec2& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const vec3& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const vec4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const mat4& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(entity_t value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void serialize(const color& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    template<class T>
    void serialize(const std::vector<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    template<class T>
    void serialize(const asset<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    template<class T>
    requires std::is_enum_v<T>
    void serialize(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void deserialize(i32& value, const rapidjson::Value& iter);
    void deserialize(u32& value, const rapidjson::Value& iter);
    void deserialize(i64& value, const rapidjson::Value& iter);
    void deserialize(u64& value, const rapidjson::Value& iter);
    void deserialize(bool& value, const rapidjson::Value& iter);
    void deserialize(f32& value, const rapidjson::Value& iter);
    void deserialize(f64& value, const rapidjson::Value& iter);
    void deserialize(std::string& value, const rapidjson::Value& iter);
    void deserialize(vec2& value, const rapidjson::Value& iter);
    void deserialize(ivec2& value, const rapidjson::Value& iter);
    void deserialize(vec3& value, const rapidjson::Value& iter);
    void deserialize(vec4& value, const rapidjson::Value& iter);
    void deserialize(mat4& value, const rapidjson::Value& iter);
    void deserialize(entity_t& value, const rapidjson::Value& iter);
    void deserialize(color& value, const rapidjson::Value& iter);
    template<class T>
    void deserialize(std::vector<T>& value, const rapidjson::Value& iter);
    template<class T>
    void deserialize(asset<T>& value, const rapidjson::Value& iter);
    template<class T>
    requires std::is_enum_v<T>
    void deserialize(T& value, const rapidjson::Value& iter);

    asset_storage& storage;
};

template<class T>
inline void basic_types_serializer::serialize(const std::vector<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartArray();
    for (const auto& elem : value)
    {
        serialize(elem, writer);
    }
    writer.EndArray();
}

template<class T>
inline void basic_types_serializer::serialize(const asset<T>& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(value.get_id(), writer);
}

template<class T>
requires std::is_enum_v<T>
inline void basic_types_serializer::serialize(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    serialize(std::underlying_type_t<T>(value), writer);
}

template<class T>
inline void basic_types_serializer::deserialize(std::vector<T>& value, const rapidjson::Value& iter)
{
    auto a = iter.GetArray();
    for (auto i = a.Begin(); i != a.End(); ++i)
    {
        T element;
        deserialize(element, i);
        value.push_back(element);
    }
}

template<class T>
inline void basic_types_serializer::deserialize(asset<T>& value, const rapidjson::Value& iter)
{
    hash_str nameid;
    deserialize(nameid, iter);
    value = storage.get_asset<T>(nameid);
}

template<class T>
requires std::is_enum_v<T>
inline void basic_types_serializer::deserialize(T& value, const rapidjson::Value& iter)
{
    auto realVal = std::underlying_type_t<T>(value);
    deserialize(realVal, iter);
    value = T(realVal);
}
}
