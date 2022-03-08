#pragma once

#include <lemon/rendering/renderer2d.h>

#include <lemon/core/string_id.h>

#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC rendering_system : public system
{
  public:
    rendering_system(ptr<scene> s);
    ~rendering_system();
    void update(entity_registry& registry) override;

  private:
    owned<renderer2d> spriteRenderer;
    entity mainCamera;
};
} // namespace lemon
