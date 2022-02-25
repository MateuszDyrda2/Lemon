#include <lemon/renderer/basic_renderer.h>

#include <lemon/math/mat4.h>
#include <lemon/math/math.h>
#include <lemon/math/vec2.h>
#include <lemon/math/vec4.h>
#include <lemon/renderer/rendering_context.h>

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
void basic_renderer::render_sprite(sprite_renderer& sComponent, transform& tComponent)
{
    auto shader  = _shader.get();
    auto texture = sComponent.text.get();
    shader->use();
    mat4 model = tComponent.model;
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
        sComponent.texCoords.x,
        sComponent.texCoords.y,
        vertices[1].x,
        vertices[1].y,
        sComponent.texCoords.x + sComponent.texCoords.z,
        sComponent.texCoords.y,
        vertices[2].x,
        vertices[2].y,
        sComponent.texCoords.x + sComponent.texCoords.z,
        sComponent.texCoords.y + sComponent.texCoords.w,
        vertices[2].x,
        vertices[2].y,
        sComponent.texCoords.x + sComponent.texCoords.z,
        sComponent.texCoords.y + sComponent.texCoords.w,
        vertices[3].x,
        vertices[3].y,
        sComponent.texCoords.x,
        sComponent.texCoords.y + sComponent.texCoords.w,
        vertices[0].x,
        vertices[0].y,
        sComponent.texCoords.x,
        sComponent.texCoords.y,
    };

    shader->set_uniform("spriteColor", sComponent.color);

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
