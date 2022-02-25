#pragma once

#include "renderer2d.h"
#include "shader.h"
#include "vertex_array.h"

#include <lemon/assets/asset.h>

namespace lemon {
class LEMON_PUBLIC basic_renderer : public renderer2d
{
  public:
    basic_renderer();
    void start_render(const mat4& viewProj) override;
    void render_sprite(sprite_renderer& sComponent, transform& tComponent) override;
    void end_render() override;

  private:
    mat4 viewProj;
    owned<vertex_array> vao;
    asset<shader> _shader;
};
} // namespace lemon
