#include <lemon/rendering/batch_renderer.h>

#include <algorithm>
#include <lemon/assets/asset.h>

namespace lemon {
batch_renderer::batch::batch():
    usedVertices{}, vao(create_owned<vertex_array>()), _texture{ nullptr }
{
    vao->add_vertex_buffer(create_owned<vertex_buffer>(batch_renderer::maxVertices * sizeof(vertex)))
        ->enable_vertex_attrib(0, 2, GL_FLOAT, false, sizeof(vertex), 0)
        ->enable_vertex_attrib(1, 4, GL_FLOAT, false, sizeof(vertex), sizeof(vec2))
        ->enable_vertex_attrib(2, 2, GL_FLOAT, false, sizeof(vertex), sizeof(vec2) + sizeof(vec4));
    vao->unbind();
}
void batch_renderer::batch::add_quad(
    const mat4& trans, vec4 color, vec4 texCoords)
{
    vec2 pos[4]   = { { -0.5f, -0.5f },
                    { 0.5f, -0.5f },
                    { 0.5f, 0.5f },
                    { -0.5f, 0.5f } };
    auto texSize  = _texture->get_size();
    auto texTrans = scale(trans, vec3(texSize.x, texSize.y, 1.0f));
    pos[0]        = texTrans * vec4(pos[0].x, pos[0].y, 0.f, 0.f);
    pos[1]        = texTrans * vec4(pos[1].x, pos[1].y, 0.f, 0.f);
    pos[2]        = texTrans * vec4(pos[2].x, pos[2].y, 0.f, 0.f);
    pos[3]        = texTrans * vec4(pos[3].x, pos[3].y, 0.f, 0.f);

    vertex vertices[6] = { { pos[0],
                             color,
                             vec2{ texCoords.x, texCoords.y } },
                           { pos[1],
                             color,
                             vec2{ texCoords.x + texCoords.z, texCoords.y } },
                           { pos[2],
                             color,
                             vec2{ texCoords.x + texCoords.z,
                                   texCoords.y + texCoords.w } },
                           { pos[2],
                             color,
                             vec2{ texCoords.x + texCoords.z,
                                   texCoords.y + texCoords.w } },
                           { pos[3],
                             color,
                             vec2{ texCoords.x, texCoords.y + texCoords.w } },
                           { pos[0],
                             color,
                             vec2{ texCoords.x, texCoords.y } } };

    vao->bind();
    vao->get_vbo()->add_subdata(usedVertices * sizeof(vertex),
                                sizeof(vertices), vertices);
    vao->unbind();
    usedVertices += 6;
}
void batch_renderer::batch::set_texture(ptr<texture> tex)
{
    _texture = tex;
}
bool batch_renderer::batch::is_texture(const ptr<texture> other) const
{
    return other == _texture;
}
ptr<batch_renderer::batch> batch_renderer::batch::get_bigger(ptr<batch> other) const
{
    return usedVertices > other->usedVertices ? ptr<batch>(this) : other;
}
bool batch_renderer::batch::is_full() const
{
    return batch_renderer::maxVertices - usedVertices < 6;
}
void batch_renderer::batch::flush(const mat4& viewProj, ptr<shader> textureShader)
{
    if(usedVertices == 0) return;
    textureShader->use();
    textureShader->set_uniform("viewProj", viewProj);
    _texture->bind();
    vao->bind();
    rendering_context::draw_arrays(GL_TRIANGLES, 0, usedVertices);
    vao->unbind();
    _texture->unbind();
    usedVertices = 0;
}
batch_renderer::batch_renderer():
    textureShader(string_id("mock_shader")),
    batches()
{
}
batch_renderer::~batch_renderer()
{
}
void batch_renderer::start_render(const mat4& viewProj)
{
    this->viewProj = viewProj;
}
void batch_renderer::render_sprite(const vec4& color, const vec4& texCoords, asset<texture>& tex,
                                   const mat4& model)
{
    static ptr<batch> biggestBatch = &batches.front();
    ptr<batch> empty{};
    ptr<batch> found{};
    for(auto& b : batches)
    {
        if(b.is_texture(tex.get()))
        {
            found = &b;
        }
        else if(b.is_empty())
        {
            empty = &b;
        }
    }
    if(found)
    {
        if(found->is_full())
            found->flush(viewProj, textureShader.get());
    }
    else if(empty)
    {
        empty->set_texture(tex.get());
        found = empty;
    }
    else
    {
        biggestBatch->flush(viewProj, textureShader.get());
        biggestBatch->set_texture(tex.get());
        found        = biggestBatch;
        biggestBatch = &batches.front();
    }
    found->add_quad(model, color, texCoords);
}
void batch_renderer::end_render()
{
    for(auto& b : batches)
        b.flush(viewProj, textureShader.get());
}
} // namespace lemon
