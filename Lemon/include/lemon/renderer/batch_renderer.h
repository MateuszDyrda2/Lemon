#pragma once

#include "rendering_context.h"
#include "shader.h"

#include <lemon/game/basic_components.h>
#include <lemon/renderer/texture.h>
#include <lemon/renderer/vertex_array.h>
#include <lemon/resources/resource_manager.h>
#include <lemon/service/services.h>

#include <array>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace lemon {
class batch_renderer
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
        resource<texture> _texture;

        batch();
        bool is_empty() const noexcept { return usedVertices == 0ULL; }
        void add_quad(const glm::mat4& trans, glm::vec4 color, glm::vec4 texCoords);
        void set_texture(resource<texture> tex);
        bool is_texture(resource<texture> other) const;
        ptr<batch> get_bigger(ptr<batch> other) const;
        bool is_full() const;
        void flush(const glm::mat4& viewProj, resource<shader> textureShader, ptr<rendering_context> context);
    };

  public:
    static constexpr size_type maxBatches  = 10;
    static constexpr size_type maxVertices = 1024;

    using container_type = std::array<batch, 10>;

  public:
    batch_renderer();
    ~batch_renderer();
    void render_sprite(const glm::mat4& viewProj, ptr<rendering_context> context,
                       sprite_renderer& sComponent, transform& tComponent);
    void end_render(const glm::mat4& viewProj, ptr<rendering_context> context);

  private:
    resource<shader> textureShader;
    container_type batches;
};
} // namespace lemon