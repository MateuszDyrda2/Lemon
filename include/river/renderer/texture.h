#pragma once

#include <river/core/basic_types.h>
#include <river/game/object.h>

namespace river {
class texture : public object
{
  public:
    texture(const std::string& name, const std::string& path);
    ~texture();

  private:
    u32 handle;
    size_type width, height;
    size_type nrOfChannels;
};
} // namespace river
