#pragma once

#include "sound_utils.h"

namespace river {
class sound_context
{
  public:
    sound_context();
    ~sound_context();

  private:
    ALCdevice* openALDevice;
    ALCcontext* openALContext;
};
} // namespace river
