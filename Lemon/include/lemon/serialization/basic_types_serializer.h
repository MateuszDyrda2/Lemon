#pragma once

#include <lemon/assets/asset.h>
#include <lemon/core/basic_types.h>
#include <lemon/core/string_id.h>
#include <lemon/game/entity.h>
#include <lemon/math/mat4.h>
#include <lemon/math/vec2.h>
#include <lemon/math/vec3.h>
#include <lemon/math/vec4.h>

#include <stringbuffer.h>
#include <writer.h>

namespace lemon {
namespace internal {
void serialize(int value, rapidjson::Writer<rapidjson::StringBuffer>& writer);
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
}
} // namespace lemon
