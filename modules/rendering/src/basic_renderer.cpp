#include <lemon/rendering/basic_renderer.h>

#include <lemon/rendering/rendering_context.h>

#include <lemon/core/math/mat4.h>
#include <lemon/core/math/math.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec4.h>

namespace lemon {
basic_renderer::basic_renderer():
    vao(create_owned<vertex_array>()),
    _shader(string_id("sprite_renderer_shader"))
{

    vao->add_vertex_buffer(
           create_owned<vertex_buffer>(sizeof(float) * 24))
        ->enable_vertex_attrib(0, 2, GL_FLOAT, false, sizeof(float) * 4, 0)
        ->enable_vertex_attrib(1, 2, GL_FLOAT, false, sizeof(float) * 4, sizeof(float) * 2);
    vao->unbind();
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

    vec4 vertices[] = {
        { -0.5f, -0.5f, 0.0f, 0.0f },
        { 0.5f, -0.5f, 0.0f, 0.0f },
        { 0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, 0.5f, 0.0f, 0.0f }
    };
    vertices[0] = model * vertices[0];
    vertices[1] = model * vertices[1];
    vertices[2] = model * vertices[2];
    vertices[3] = model * vertices[3];

    float v[] = {
        vertices[0].x,
        vertices[0].y,
        texCoords.x,
        texCoords.y,
        vertices[1].x,
        vertices[1].y,
        texCoords.x + texCoords.z,
        texCoords.y,
        vertices[2].x,
        vertices[2].y,
        texCoords.x + texCoords.z,
        texCoords.y + texCoords.w,
        vertices[2].x,
        vertices[2].y,
        texCoords.x + texCoords.z,
        texCoords.y + texCoords.w,
        vertices[3].x,
        vertices[3].y,
        texCoords.x,
        texCoords.y + texCoords.w,
        vertices[0].x,
        vertices[0].y,
        texCoords.x,
        texCoords.y,
    };

    shader->set_uniform("spriteColor", color);

    texture->bind();
    vao->bind();
    vao->get_vbo()->add_subdata(0, sizeof(v), v);
    rendering_context::draw_arrays(GL_TRIANGLES, 0, 6);
    vao->unbind();
}
void basic_renderer::end_render()
{
}
} // namespace lemon
