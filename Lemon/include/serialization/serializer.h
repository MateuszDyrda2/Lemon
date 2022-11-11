#pragma once

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"
#include "serialization/basic_types_serializer.h"
#include "world/scene.h"
#include "world/system.h"
#include <core/defines.h>

#include <unordered_map>

namespace lemon {

class LEMON_API serializer
{
  public:
    serializer(asset_storage& storage);

    struct serialization_data
    {
        const char* name;
        unsigned int id;
        void (*serialize_f)(serializer*, const void*, rapidjson::Writer<rapidjson::StringBuffer>&);
        void (*deserialize_f)(serializer*, void*, const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& obj);
    };

    std::unordered_map<u32, serialization_data> serializationData;

    void serialize_scene(const scene& _scene, const char* path);
    void serialize(const registry& _registry, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    void deserialize_scene(scene& _scene, const char* path);
    void deserialize(registry& _registry, rapidjson::Document& document);
    void register_all(registry& _registry);

  private:
    basic_types_serializer basicTypesSerializer;
};
}
