#pragma once

#include <lemon/scene/system.h>

#include <lemon/core/basic_types.h>
#include <lemon/scene/scene.h>

namespace lemon {
class LEMON_PUBLIC debug_system : public system
{
  public:
    debug_system(ptr<scene> s);
    ~debug_system();
    void update(entity_registry& registry) override;

  private:
    bool enabled;
    bool showColliders;
    bool showFPS;
};
} // namespace lemon
