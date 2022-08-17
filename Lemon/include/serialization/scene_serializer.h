#pragma once

#include <core/defines.h>
#include <string>
#include <world/scene.h>

namespace lemon {
class LEMON_API scene_serializer
{
  public:
    static void serialize(scene* s, const std::string& path);
    static registry&& deserialize(const std::string& path);
};
}