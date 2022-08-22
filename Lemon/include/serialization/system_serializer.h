#pragma once

#include <world/scene.h>
#include <world/system.h>

#include "basic_types_serializer.h"

namespace lemon {
using namespace rapidjson;
inline static void
serialize_systems(
    const std::array<std::vector<std::unique_ptr<system>>, 4>& systems,
    Writer<StringBuffer>& writer)
{
    size_t i = 0;
    for(const auto& stage : systems)
    {
        writer.StartObject();
        writer.Key("stage");
        writer.Int(i++);
        writer.Key("systems");
        writer.StartArray();
        for(const auto& system : stage)
        {
            serialize(system->get_nameid(), writer);
        }
        writer.EndArray();
        writer.EndObject();
    }
}

inline static void
deserialize_systems()
{
}
}
