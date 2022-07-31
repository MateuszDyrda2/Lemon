#pragma once

#include "renderer2d.h"
#include "shader.h"
#include "vertex_array.h"

namespace lemon {
/** Basic texture 2D renderer */
class basic_renderer : public renderer2d
{
  public:
    /** @brief Creates the renderer */
    basic_renderer(asset_storage& storage);
    virtual ~basic_renderer() = default;
    /** @see renderer2d::start_render */
    void start_render(const mat4& viewProj) override;
    /** @see renderer2d::render_sprite */
    void render_sprite(const vec4& color, const vec4& texCoords, asset<texture>& tex,
                       const mat4& model) override;
    /** @see renderer2d::end_render */
    void end_render() override;

  private:
    mat4 viewProj;
    u32 vao, vbo1, vbo2;
    asset<shader> _shader;
};
} // namespace lemon
