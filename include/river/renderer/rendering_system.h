#pragma once

#include "batch_renderer.h"

#include <river/core/string_id.h>
#include <river/event_system/event_handler.h>

#include <entt/entity/registry.hpp>

namespace river {
class scene;
class rendering_system
{
  public:
    rendering_system(ptr<scene> s, ptr<rendering_context> context);
    ~rendering_system();
    void render(entity_registry& registry);

  private:
    owned<batch_renderer> spriteRenderer;
    ptr<rendering_context> context;
    entity mainCamera;
    ptr<event_handler> handler;
    unsubscriber<int, int> vpResize;
};
} // namespace river
