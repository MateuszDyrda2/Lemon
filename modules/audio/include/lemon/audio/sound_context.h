#pragma once

#include "detail/sound_utils.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

namespace lemon {
class LEMON_PUBLIC sound_context
{
  public:
    static void create();
    static void drop();

  private:
    static ptr<ALCdevice> openALDevice;
    static ptr<ALCcontext> openALContext;
};
} // namespace lemon
