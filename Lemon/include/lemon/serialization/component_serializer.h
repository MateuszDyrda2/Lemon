#pragma once

#include "basic_types_serializer.h"

#include <lemon/game/basic_components.h>

#define CONCATINATE(arg1, arg2)  CONCATINATE1(arg1, arg2)
#define CONCATINATE1(arg1, arg2) CONCATINATE2(arg1, arg2)
#define CONCATINATE2(arg1, arg2) arg1##arg2

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define SERIALIZE_BASIC(__obj, __name, __writer) \
    __writer.Key(STRINGIZE(__name));             \
    internal::serialize(__obj.__name, __writer)

#define SERIALIZE_COMPONENT(__name, ...)                                 \
    inline void                                                          \
    __serialize_comp(rapidjson::Writer<rapidjson::StringBuffer>& writer) \
    {                                                                    \
        writer.StartObject();                                            \
        writer.Key(STRINGIZE(__name));                                   \
        SERIALIZE_N(writer, __VA_ARGS__)                                 \
        internal::serialize(this->__value1, writer);                     \
        writer.EndObject();                                              \
    }

#define REGISTER_COMPONENT(__name, ...)                                                                      \
    template<>                                                                                               \
    inline void                                                                                              \
    serialize_component<__name>(const __name& component, rapidjson::Writer<rapidjson::StringBuffer>& writer) \
    {                                                                                                        \
        writer.StartObject();                                                                                \
        SERIALIZE_N(component, writer, __VA_ARGS__);                                                         \
        writer.EndObject();                                                                                  \
    }

#define SERIALIZE_0(__obj, _writer)
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
    (__obj, __writer, __VA_ARGS__);

namespace lemon {
template<class T>
void serialize_component(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    writer.EndObject();
}
REGISTER_COMPONENT(tag, id);
REGISTER_COMPONENT(transform, position, scale, rotation, first, next, parent, order);
REGISTER_COMPONENT(camera, viewport);
REGISTER_COMPONENT(sprite_renderer, color, texCoords, text);
}
/*
namespace lemon {
template<class T>
void serialize_component(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    writer.EndObject();
}
template<>
inline void serialize_component<tag>(const tag& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    internal::serialize(component.id, writer);
    writer.EndObject();
}
template<>
inline void serialize_component<transform>(const transform& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    internal::serialize(component.position, writer);
    internal::serialize(component.scale, writer);
    internal::serialize(component.rotation, writer);
    internal::serialize(component.model, writer);
    internal::serialize(component.first, writer);
    internal::serialize(component.next, writer);
    internal::serialize(component.parent, writer);
    internal::serialize(component.order, writer);
    writer.EndObject();
}
template<>
inline void serialize_component<camera>(const camera& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    internal::serialize(component.viewport, writer);
    internal::serialize(component.projection, writer);
    writer.EndObject();
}
template<>
inline void
serialize_component<sprite_renderer>(const sprite_renderer& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.StartObject();
    internal::serialize(component.color, writer);
    internal::serialize(component.texCoords, writer);
    internal::serialize(component.text, writer);
    writer.EndObject();
}
} // namespace lemon
*/
