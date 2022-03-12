#include <lemon/rendering/basic_renderer.h>

#include <lemon/rendering/rendering_context.h>

#include <lemon/core/math/mat4.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

namespace lemon {
basic_renderer::basic_renderer():
    // vao(create_owned<vertex_array>()),
    _shader(string_id("sprite_renderer_shader"))
{
    /// struct vertex
    /// {
    ///     vec2 pos;
    ///     vec2 texCoords;
    /// } vertices[]{
    ///     { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
    ///     { { 0.5f, -0.5f }, { 1.0f, 0.0f } },
    ///     { { 0.5f, 0.5f }, { 1.0f, 1.0f } },
    ///     { { 0.5f, 0.5f }, { 1.0f, 1.0f } },
    ///     { { -0.5f, 0.5f }, { 0.0f, 1.0f } },
    ///     { { -0.5f, -0.5f }, { 0.0f, 0.0f } },
    /// };
    /// vao->add_vertex_buffer(
    ///        create_owned<vertex_buffer>(&vertices, sizeof(vertices)))
    ///     ->enable_vertex_attrib(0, 2, GL_FLOAT, false, sizeof(float) * 4, 0)
    ///     ->enable_vertex_attrib(1, 2, GL_FLOAT, false, sizeof(float) * 4, sizeof(float) * 2);
    /// vao->unbind();
    glCreateVertexArrays(1, &vao);
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
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 12, nullptr, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
void basic_renderer::start_render(const mat4& viewProj)
{
    this->viewProj = viewProj;
    auto shader    = _shader.get();
    shader->use();
    shader->set_uniform("viewProj", viewProj);
}
void basic_renderer::render_sprite(const vec4& color, const vec4& texCoords, asset<texture>& tex,
                                   const mat4& m)
{
    auto shader  = _shader.get();
    auto texture = tex.get();
    shader->use();
    mat4 model = m;
    model      = scale(model, vec3(texture->get_size(), 1.0f));

    shader->set_uniform("spriteColor", color);
    shader->set_uniform("model", model);

    texture->bind();
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    f32 tc[] = {
        texCoords.x, texCoords.y,
        texCoords.z, texCoords.y,
        texCoords.z, texCoords.w,
        texCoords.z, texCoords.w,
        texCoords.x, texCoords.w,
        texCoords.x, texCoords.y
    };
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tc), tc);
    rendering_context::draw_arrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void basic_renderer::end_render()
{
}
} // namespace lemon
