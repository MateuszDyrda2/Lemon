#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/engine/scene/scene.h>

#include <string>

namespace lemon {
class LEMON_PUBLIC scene_serializer
{
  public:
    static void serialize(ptr<scene> scene);
    static ptr<scene> deserialize(const std::string& name);
};
} // namespace lemon
