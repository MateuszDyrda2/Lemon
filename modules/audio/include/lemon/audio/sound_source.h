#pragma once

#include "detail/sound_utils.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

namespace lemon {
class LEMON_PUBLIC sound_source
{
  public:
    sound_source();
    ~sound_source();
    [[nodiscard]] u32 get_handle() const { return handle; }

  private:
    ALuint handle;
};
} // namespace lemon
