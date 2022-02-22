#pragma once

#include "component_serializer.h"

#include <lemon/core/basic_types.h>
#include <lemon/game/scene.h>

#include <entt/entt.hpp>
#include <string>

namespace lemon {
class scene_serializer
{
  public:
    scene_serializer();
    ~scene_serializer();
    ptr<scene_serializer> serialize(ptr<scene> scene);
    ptr<scene_serializer> save2file(const std::string& path);

  private:
    struct output_archive
    {
        output_archive();
        void start();
        void end();
        void operator()(entt::entity entity);
        void operator()(std::underlying_type_t<entt::entity> size);
        template<class T>
        void operator()(entt::entity entity, const T& component);

        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer;
        int started;
    };

  private:
    output_archive output;
};
class scene_deserializer
{
  public:
    scene_deserializer();
    ~scene_deserializer();
    ptr<scene_deserializer> read_file(const std::string& path);
    ptr<scene_deserializer> deserialize(ptr<scene> scene);

  private:
    struct input_archive
    {
        input_archive();
        void operator()(entt::entity& entity);
        void operator()(std::underlying_type_t<entt::entity>& size);
        template<class T>
        void operator()(entt::entity& entity, T& component);

        rapidjson::Document document;
        rapidjson::Value::ConstValueIterator enttIterator;
        rapidjson::Value::ConstValueIterator endIterator;
        rapidjson::Value::ConstValueIterator componentIterator;
        int started;
    };

  private:
    input_archive input;
};
} // namespace lemon
