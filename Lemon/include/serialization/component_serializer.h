#pragma once

#include "basic_types_serializer.h"

#include <world/reflection.h>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace lemon {
template<class T>
inline void serialize_component(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    writer.Key(reflection::name<T>());
    {
        reflection::for_each(component, [&](const auto& field) {
            serialize(field, writer);
        });
    }
    writer.EndObject();
}
template<class T>
inline void deserialize_component(T& component, rapidjson::Value::ConstMemberIterator& iter)
{
    reflection::for_each(component, [&](auto& field) {
        deserialize(field, iter->value);
    });
}
}
