#pragma once

#include "sound_utils.h"
#include <lemon/core/defines.h>

namespace lemon {
class LEMON_PUBLIC sound_context
{
  public:
    sound_context();
    ~sound_context();

  private:
    ALCdevice* openALDevice;
    ALCcontext* openALContext;
};
} // namespace lemon
