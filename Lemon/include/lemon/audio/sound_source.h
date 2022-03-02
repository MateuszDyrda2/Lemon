#pragma once

#include "sound_utils.h"

namespace lemon {
class sound_source
{
  public:
    sound_source();
    ~sound_source();
    [[nodiscard]] u32 get_handle() const { return handle; }

  private:
    ALuint handle;
};
} // namespace lemon
