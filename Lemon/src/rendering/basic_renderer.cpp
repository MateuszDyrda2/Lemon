#include <rendering/basic_renderer.h>

#include <rendering/rendering_context.h>

#include <rendering/gl_errors.h>

#include <core/math/mat4.h>
#include <core/math/math.h>
#include <core/math/vec2.h>
#include <core/math/vec4.h>

namespace lemon {
basic_renderer::basic_renderer(asset_storage& storage):
    renderer2d(storage),
    _shader(storage.get_asset<shader>("sprite_renderer_shader"_hs))
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    u32 buffers[2];
    glGenBuffers(2, buffers);
    vbo1           = buffers[0];
    vbo2           = buffers[1];
    f32 vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
        -0.5f, -0.5f
    };
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo1));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void*)0));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo2));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 12, nullptr, GL_STREAM_DRAW));
    GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0));
    GL_CHECK(glEnableVertexAttribArray(1));
    glBindVertexArray(0);
}
void basic_renderer::start_render(const mat4& viewProj)
{
    this->viewProj = viewProj;
    auto shader    = _shader.get();
    shader->use();
    shader->set_uniform("viewProj", viewProj);
}
void basic_renderer::render_sprite(const color& col, const vec4& texCoords, asset<texture>& tex,
                                   const mat4& m)
{
    auto shader  = _shader.get();
    auto texture = tex.get();
    shader->use();
    auto model         = m;
    const auto texSize = texture->get_size();
    const f32 texW     = f32(texSize.x);
    const f32 texH     = f32(texSize.y);

    model = scale(model, vec3(texW, texH, 1.f));

    shader->set_uniform("spriteColor", col.rgba);
    shader->set_uniform("model", model);

    texture->bind();
    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo2));
    f32 tc[] = {
        texCoords.x, texCoords.y,
        texCoords.z, texCoords.y,
        texCoords.z, texCoords.w,
        texCoords.z, texCoords.w,
        texCoords.x, texCoords.w,
        texCoords.x, texCoords.y
    };
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tc), tc));
    rendering_context::draw_arrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void basic_renderer::end_render()
{
}
} // namespace lemon
