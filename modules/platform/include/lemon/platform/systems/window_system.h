#pragma once

#include <lemon/core/basic_types.h>
#include <lemon/core/defines.h>
#include <lemon/scene/system.h>

#include "../window.h"

namespace lemon {
class LEMON_PUBLIC window_system : public system
{
  public:
    window_system();
    ~window_system();
    void update(entity_registry& registry) override;

  private:
    window mainWindow;
};
} // namespace lemon
