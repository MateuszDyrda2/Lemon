#pragma once

#include "opengl_context.h"
#include "shader.h"

#include <river/game/basic_components.h>
#include <river/resources/resource_manager.h>

#include <array>

namespace river {
class batch_renderer
{
  public:
    static constexpr size_type maxBatches  = 10;
    static constexpr size_type maxVertices = 1024;

    using container_type = std::array<owned<class batch>, 10>;

  public:
    batch_renderer()  = default;
    ~batch_renderer() = default;
    void render_sprite(const glm::mat4& viewProj, ptr<opengl_context> context,
                       sprite_renderer& sComponent, transform& tComponent);
    void end_render(const glm::mat4& viewProj, ptr<opengl_context> context);

  private:
    container_type batches;
    resource<shader> textureShader;
};
} // namespace river