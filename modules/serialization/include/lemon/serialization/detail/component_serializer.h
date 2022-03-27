#pragma once

#include "basic_types_serializer.h"

#include <lemon/scene/reflection.h>
/*
#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define SERIALIZE_BASIC(__obj, __name, __writer) \
    __writer.Key(STRINGIZE(__name));             \
    internal::serialize(__obj.__name, __writer)

#define DESERIALIZE_BASIC(__obj, __name, __iter)        \
    internal::deserialize(__obj.__name, __iter->value); \
    ++__iter

#define REGISTER_COMPONENT(__name, ...)                                                                      \
    template<>                                                                                               \
    inline void                                                                                              \
    serialize_component<__name>(const __name& component, rapidjson::Writer<rapidjson::StringBuffer>& writer) \
    {                                                                                                        \
        writer.StartObject();                                                                                \
        SERIALIZE_N(component, writer, __VA_ARGS__);                                                         \
        writer.EndObject();                                                                                  \
    }                                                                                                        \
    template<>                                                                                               \
    inline void                                                                                              \
    deserialize_component<__name>(__name & component, rapidjson::Value::ConstMemberIterator & iterator)      \
    {                                                                                                        \
        DESERIALIZE_N(component, iterator, __VA_ARGS__);                                                     \
    }

#define DESERIALIZE_0(__obj, __iter)
#define DESERIALIZE_1(__obj, __iter, __value1) \
    DESERIALIZE_BASIC(__obj, __value1, __iter)

#define DESERIALIZE_2(__obj, __iter, __value1, __value2) \
    DESERIALIZE_1(__obj, __iter, __value1);              \
    DESERIALIZE_BASIC(__obj, __value2, __iter)

#define DESERIALIZE_3(__obj, __iter, __value1, __value2, __value3) \
    DESERIALIZE_2(__obj, __iter, __value1, __value2);              \
    DESERIALIZE_BASIC(__obj, __value3, __iter)

#define DESERIALIZE_4(__obj, __iter, __value1, __value2, __value3, __value4) \
    DESERIALIZE_3(__obj, __iter, __value1, __value2, __value3);              \
    DESERIALIZE_BASIC(__obj, __value4, __iter)

#define DESERIALIZE_5(__obj, __iter, __value1, __value2, __value3, __value4, __value5) \
    DESERIALIZE_4(__obj, __iter, __value1, __value2, __value3, __value4);              \
    DESERIALIZE_BASIC(__obj, __value5, __iter)

#define DESERIALIZE_6(__obj, __iter, __value1, __value2, __value3, __value4, __value5, __value6) \
    DESERIALIZE_5(__obj, __iter, __value1, __value2, __value3, __value4, __value5);              \
    DESERIALIZE_BASIC(__obj, __value6, __iter)

#define DESERIALIZE_7(__obj, __iter, __value1, __value2, __value3, __value4, __value5, __value6, __value7) \
    DESERIALIZE_6(__obj, __iter, __value1, __value2, __value3, __value4, __value5, __value7);              \
    DESERIALIZE_BASIC(__obj, __value7, __iter)

#define SERIALIZE_0(__obj, __writer)
#define SERIALIZE_1(__obj, __writer, __value1) \
    SERIALIZE_BASIC(__obj, __value1, __writer)

#define SERIALIZE_2(__obj, __writer, __value1, __value2) \
    SERIALIZE_1(__obj, __writer, __value1);              \
    SERIALIZE_BASIC(__obj, __value2, __writer)

#define SERIALIZE_3(__obj, __writer, __value1, __value2, __value3) \
    SERIALIZE_2(__obj, __writer, __value1, __value2);              \
    SERIALIZE_BASIC(__obj, __value3, __writer)

#define SERIALIZE_4(__obj, __writer, __value1, __value2, __value3, __value4) \
    SERIALIZE_3(__obj, __writer, __value1, __value2, __value3);              \
    SERIALIZE_BASIC(__obj, __value4, __writer)

#define SERIALIZE_5(__obj, __writer, __value1, __value2, __value3, __value4, __value5) \
    SERIALIZE_4(__obj, __writer, __value1, __value2, __value3, __value4);              \
    SERIALIZE_BASIC(__obj, __value5, __writer)

#define SERIALIZE_6(__obj, __writer, __value1, __value2, __value3, __value4, __value5, __value6) \
    SERIALIZE_5(__obj, __writer, __value1, __value2, __value3, __value4, __value5);              \
    SERIALIZE_BASIC(__obj, __value6, __writer)

#define SERIALIZE_7(__obj, __writer, __value1, __value2, __value3, __value4, __value5, __value6, __value7) \
    SERIALIZE_6(__obj, __writer, __value1, __value2, __value3, __value4, __value5, __value6);              \
    SERIALIZE_BASIC(__obj, __value7, __writer)

#define GET_MACRO(_0, _1, _2, _3, _4, _5, _6, _7, NAME, ...) NAME
#define SERIALIZE_N(__obj, __writer, ...)                                                                                              \
    GET_MACRO(_0, __VA_ARGS__, SERIALIZE_7, SERIALIZE_6, SERIALIZE_5, SERIALIZE_4, SERIALIZE_3, SERIALIZE_2, SERIALIZE_1, SERIALIZE_0) \
    (__obj, __writer, __VA_ARGS__)

#define DESERIALIZE_N(__obj, __iter, ...)                                                                                                                \
    GET_MACRO(_0, __VA_ARGS__, DESERIALIZE_7, DESERIALIZE_6, DESERIALIZE_5, DESERIALIZE_4, DESERIALIZE_3, DESERIALIZE_2, DESERIALIZE_1, DESERIALIZE_0, ) \
    (__obj, __iter, __VA_ARGS__)
*/
namespace lemon {
/*
template<class T>
void serialize_component(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    writer.EndObject();
}
template<class T>
void deserialize_component(T& component, rapidjson::Value::ConstMemberIterator& iter)
{
}
REGISTER_COMPONENT(tag, id);
REGISTER_COMPONENT(transform, position, scale, rotation, first, next, parent, order);
REGISTER_COMPONENT(camera, viewport);
REGISTER_COMPONENT(sprite_renderer, col, texCoords, text);
*/
template<class T>
void serialize_component(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    {
        reflection::for_each(component, [&](const auto& field) {
            internal::serialize(field, writer);
        });
    }
    writer.EndObject();
}
template<class T>
void deserialize_component(T& component, rapidjson::Value::ConstMemberIterator& iter)
{
    reflection::for_each(component, [&](auto& field) {
        internal::deserialize(field, iter->value);
    });
}
}
