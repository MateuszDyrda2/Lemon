#pragma once

#include "batch_renderer.h"

#include <lemon/core/string_id.h>
#include <lemon/events/listener.h>

#include <lemon/game/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC rendering_system : public system
{
  public:
    rendering_system(ptr<scene> s);
    ~rendering_system();
    void update(entity_registry& registry) override;

  private:
    owned<batch_renderer> spriteRenderer;
    entity mainCamera;
    listener<int, int> vpResize;
};
} // namespace lemon
