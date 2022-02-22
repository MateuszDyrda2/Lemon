#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/game/scene.h>

#include <string>

namespace lemon {
class scene_serializer
{
  public:
    static void serialize(ptr<scene> scene);
    static ptr<scene> deserialize(const std::string& name);
};
} // namespace lemon
