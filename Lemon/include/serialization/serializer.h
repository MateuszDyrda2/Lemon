/** @file serializer.h
 * @brief File with a definion of a scene serializer
 */
#pragma once

#include "rapidjson/document.h"
#include "rapidjson/encodings.h"
#include "serialization/basic_types_serializer.h"
#include "world/scene.h"
#include "world/system.h"
#include <core/defines.h>

#include <unordered_map>

namespace lemon {
/** Class for serializing / deserializing a scene */
class LEMON_API serializer
{
  public:
    /** @brief Creates a serializer
     * @param storage asset storage reference
     */
    serializer(asset_storage& storage);

    struct serialization_data
    {
        const char* name;
        unsigned int id;
        void (*serialize_f)(serializer*, const void*, rapidjson::Writer<rapidjson::StringBuffer>&);
        void (*deserialize_f)(serializer*, void*, const rapidjson::GenericValue<rapidjson::UTF8<char>>::ConstObject& obj);
    };

    std::unordered_map<u32, serialization_data> serializationData;
    std::unordered_map<u32, u32> componentIds;

    /** @brief Serializes a scene to a file
     * @param _scene scene reference
     * @param path path to serialize to
     */
    void serialize_scene(const scene& _scene, const char* path);
    /** @brief Serialization implementation
     * @param _registry entity registry to serialize
     * @param writer rapidjson writer
     */
    void serialize(const registry& _registry, rapidjson::Writer<rapidjson::StringBuffer>& writer);
    /** @brief Deserializes scene from a file
     * @param _scene scene to deserialize
     * @param path path to deserialize from
     */
    void deserialize_scene(scene& _scene, const char* path);
    /** @brief Deserialization implementation
     * @param _registry entity registry to deserialize to
     * @param document rapidjson doc
     */
    void deserialize(registry& _registry, rapidjson::Document& document);
    /** @brief Register all types
     * @param _registry
     */
    void register_all(registry& _registry);

  private:
    basic_types_serializer basicTypesSerializer;
};
}
