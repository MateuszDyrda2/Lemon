#pragma once

#include "renderer2d.h"
#include "shader.h"
#include "vertex_array.h"

#include <lemon/assets/asset.h>

namespace lemon {
/** Basic texture 2D renderer */
class LEMON_PUBLIC basic_renderer : public renderer2d
{
  public:
    /** @brief Creates the renderer */
    basic_renderer();
    virtual ~basic_renderer() = default;
    /** @see renderer2d::start_render */
    void start_render(const mat4& viewProj) override;
    /** @see renderer2d::render_sprite */
    void render_sprite(sprite_renderer& sComponent, transform& tComponent) override;
    /** @see renderer2d::end_render */
    void end_render() override;

  private:
    mat4 viewProj;
    owned<vertex_array> vao;
    asset<shader> _shader;
};
} // namespace lemon
