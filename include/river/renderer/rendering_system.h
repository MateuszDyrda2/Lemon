#pragma once

#include "batch_renderer.h"
#include "opengl_context.h"

#include <river/core/string_id.h>

#include <entt/entity/registry.hpp>

namespace river {
class rendering_system
{
  public:
    rendering_system(entity_registry& registry);
    ~rendering_system();
    void render(entity_registry& registry, entity mainCamera);

  private:
    owned<batch_renderer> spriteRenderer;
    owned<opengl_context> context;
};
} // namespace river
