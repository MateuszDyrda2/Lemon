#pragma once

#include "renderer2d.h"
#include "rendering_context.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

#include <lemon/core/math/mat4.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>

#include <array>

namespace lemon {
/** 2D texture renderer working in batches */
class batch_renderer : public renderer2d
{
    struct vertex
    {
        vec2 position;
        vec4 color;
        vec2 texCoords;
    };
    struct batch
    {
        std::size_t usedVertices;
        std::unique_ptr<vertex_array> vao;
        texture* _texture;

        batch();
        bool is_empty() const noexcept { return usedVertices == 0ULL; }
        void add_quad(const mat4& trans, vec4 color, vec4 texCoords);
        void set_texture(texture* tex);
        bool is_texture(const texture* other) const;
        batch* get_bigger(batch* other) const;
        bool is_full() const;
        void flush(const mat4& viewProj, shader* textureShader);
    };

  public:
    static constexpr std::size_t maxBatches  = 10;
    static constexpr std::size_t maxVertices = 1024;

    using container_type = std::array<batch, maxBatches>;

  public:
    /** @brief Creates the batch renderer */
    batch_renderer(asset_storage& storage);
    virtual ~batch_renderer();
    /** @see renderer2d::start_render */
    void start_render(const mat4& viewProj) override;
    /** @see renderer2d::render_sprite */
    void render_sprite(const vec4& color, const vec4& texCoords, asset<texture>& tex,
                       const mat4& model) override;
    /** @see renderer2d::end_render */
    void end_render() override;

  private:
    asset<shader> textureShader;
    container_type batches;
    mat4 viewProj;
};
} // namespace lemon