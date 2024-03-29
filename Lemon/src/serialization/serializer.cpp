#include "entt/core/type_info.hpp"
#include "physics/components/physics_components.h"
#include "serialization/basic_types_serializer.h"
#include "world/components/transform_components.h"
#include <fstream>
#include <serialization/serializer.h>
#include <type_traits>

namespace lemon {

serializer::serializer(asset_storage& storage):
    basicTypesSerializer{ .storage = storage }
{
}

void serializer::serialize_scene(const scene& _scene, const char* path)
{
    const auto& registry = _scene.get_registry();
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();
    serialize(registry, writer);
    writer.EndObject();

    std::ofstream f;
    f.open(path, std::ios::binary);
    if (!f)
    {
        logger::error("Failed to create file {}", path);
        return;
    }
    f.write(buffer.GetString(), buffer.GetSize());
    f.close();
}

void serializer::deserialize_scene(scene& _scene, const char* path)
{
    auto& registry = _scene.get_registry();
    std::ifstream f;
    f.open(path, std::ios::binary | std::ios::ate);
    if (!f)
    {
        logger::error("Failed to open file {}", path);
        return;
    }
    std::size_t nbBytes = f.tellg();
    f.seekg(0, f.beg);
    std::vector<char> buffer(nbBytes);
    f.read(buffer.data(), nbBytes);
    f.close();

    buffer.push_back('\0');
    rapidjson::Document document;
    document.ParseInsitu(buffer.data());
    deserialize(registry, document);
}

void serializer::serialize(const registry& _registry, rapidjson::Writer<rapidjson::StringBuffer>& writer)
{
    writer.Key("entities");
    writer.StartObject();
    writer.Key("count");
    writer.Uint(u32(_registry.size()));
    writer.Key("ids");
    writer.StartArray();
    _registry.each([&writer](const entity_t e) {
        writer.Uint(std::underlying_type_t<decltype(e)>(e));
    });
    writer.EndArray();
    writer.EndObject();
    auto storage = _registry.storage();
    writer.Key("components");
    writer.StartArray();
    for (auto&& ss : storage)
    {
        auto id = ss.first;
        if (!serializationData.contains(id)) continue;
        auto& sparseSet                       = ss.second;
        auto&& [name, nameid, serialize_f, _] = serializationData[id];
        auto size                             = sparseSet.size();

        writer.StartObject();
        writer.Key("name");
        writer.String(name);
        writer.Key("id");
        writer.Uint(hashstr::runtime_hash(name).value);
        writer.Key("count");
        writer.Uint(u32(size));
        writer.Key("entities");
        writer.StartObject();
        for (auto&& c : sparseSet)
        {
            writer.Key(std::to_string(std::underlying_type_t<entt::entity>(c)).c_str());
            writer.StartObject();
            const auto&& component = sparseSet.get(c);
            serialize_f(this, component, writer);
            writer.EndObject();
        }

        writer.EndObject();
        writer.EndObject();
    }
    writer.EndArray();
}

void serializer::deserialize(registry& _registry, rapidjson::Document& document)
{
    // Entities
    const rapidjson::Value& a = document["entities"];
    auto entitiesC            = a.FindMember("count");
    if (entitiesC == a.MemberEnd())
    {
        logger::error("Failed to load file. No entity count present");
        return;
    }
    auto nbEntities = entitiesC->value.GetUint();
    _registry.reserve(nbEntities);
    auto entityids = a.FindMember("ids");
    if (entityids == a.MemberEnd())
    {
        logger::error("Failed to load file. No entities present");
        return;
    }

    for (auto& v : entityids->value.GetArray())
    {
        auto entityid   = v.GetUint();
        auto returnedId = _registry.create(entity_t(entityid));
        if (entityid != std::underlying_type_t<decltype(returnedId)>(returnedId))
        {
            logger::warn("Returned id is not equal the one passed from file");
            return;
        }
    }

    // Components
    const rapidjson::Value& b = document["components"];
    for (auto& v : b.GetArray())
    {
        auto&& c      = v.GetObject();
        auto count    = c.FindMember("count");
        auto id       = c.FindMember("id");
        auto entities = c.FindMember("entities");

        if (count == c.MemberEnd())
        {
            logger::error("Failed to load file: Component contains no count");
            return;
        }

        if (id == c.MemberEnd())
        {
            logger::error("Failed to load file: Component contains no id");
            return;
        }

        if (entities == c.MemberEnd())
        {
            logger::error("Failed to load file: Component contains no entities");
            return;
        }
        auto hashid_value = id->value.GetUint();
        auto realid_value = componentIds[hashid_value];
        auto storage = _registry.storage(realid_value);
        if (!storage)
        {
            logger::error("Storage not existing");
            return;
        }
        storage->reserve(count->value.GetUint());

        for (auto&& vv : entities->value.GetObject())
        {
            u32 entityid   = u32(atoi(vv.name.GetString()));
            auto component = vv.value.GetObject();

            storage->emplace(entity_t(entityid));
            auto inserted        = storage->get(entity_t(entityid));
            auto&& deserialize_f = serializationData[realid_value].deserialize_f;
            deserialize_f(this, inserted, component);
        }
    }

    _registry.view<rigidbody, transform>().each([](auto, auto& rb, auto& tr) {
        rb.position = rb._oldPosition = tr.position;
    });

    _registry.each([&_registry](const auto ent) {
        _registry.emplace<dirty_t>(ent);
        _registry.emplace<model>(ent);
    });
}
}
