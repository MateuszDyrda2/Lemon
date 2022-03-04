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
class LEMON_PUBLIC batch_renderer : public renderer2d
{
    struct vertex
    {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 texCoords;
    };
    struct batch
    {
        size_type usedVertices;
        owned<vertex_array> vao;
        ptr<texture> _texture;

        batch();
        bool is_empty() const noexcept { return usedVertices == 0ULL; }
        void add_quad(const mat4& trans, vec4 color, vec4 texCoords);
        void set_texture(ptr<texture> tex);
        bool is_texture(const ptr<texture> other) const;
        ptr<batch> get_bigger(ptr<batch> other) const;
        bool is_full() const;
        void flush(const mat4& viewProj, ptr<shader> textureShader);
    };

  public:
    static constexpr size_type maxBatches  = 10;
    static constexpr size_type maxVertices = 1024;

    using container_type = std::array<batch, maxBatches>;

  public:
    /** @brief Creates the batch renderer */
    batch_renderer();
    virtual ~batch_renderer();
    /** @see renderer2d::start_render */
    void start_render(const glm::mat4& viewProj) override;
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