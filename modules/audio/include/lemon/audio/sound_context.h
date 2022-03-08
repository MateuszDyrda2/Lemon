#pragma once

#include "detail/sound_utils.h"

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>

namespace lemon {
class LEMON_PUBLIC sound_context
{
  public:
    sound_context();
    ~sound_context();

  private:
    static ptr<ALCdevice> openALDevice;
    static ptr<ALCcontext> openALContext;

  private:
    static void create();
    static void drop();
};
} // namespace lemon
