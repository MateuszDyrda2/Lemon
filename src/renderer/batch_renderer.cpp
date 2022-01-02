#include <river/renderer/batch_renderer.h>

#include <algorithm>
#include <river/resources/resource_manager.h>

namespace river {
batch_renderer::batch::batch():
    usedVertices{}, vao(create_owned<vertex_array>()), _texture{ nullptr }
{
    vao->add_vertex_buffer(create_owned<vertex_buffer>(batch_renderer::maxVertices * sizeof(vertex)));
    vao->get_vbo()->enable_vertex_attrib(0, 2, GL_FLOAT, false, sizeof(vertex), 0);
    vao->get_vbo()->enable_vertex_attrib(1, 4, GL_FLOAT, false, sizeof(vertex), sizeof(glm::vec2));
    vao->get_vbo()->enable_vertex_attrib(2, 2, GL_FLOAT, false, sizeof(vertex), sizeof(glm::vec2) + sizeof(glm::vec4));
    vao->unbind();
}
void batch_renderer::batch::add_quad(const glm::mat4& trans, glm::vec4 color, glm::vec4 texCoords)
{
    glm::vec2 pos[4] = { { -0.5f, -0.5f },
                         { 0.5f, -0.5f },
                         { 0.5f, 0.5f },
                         { -0.5f, 0.5f } };
    auto texSize     = _texture->get_size();
    auto texTrans    = glm::scale(trans, glm::vec3(texSize.x, texSize.y, 1.0f));
    pos[0]           = texTrans * glm::vec4(pos[0].x, pos[0].y, 0.f, 0.f);
    pos[1]           = texTrans * glm::vec4(pos[1].x, pos[1].y, 0.f, 0.f);
    pos[2]           = texTrans * glm::vec4(pos[2].x, pos[2].y, 0.f, 0.f);
    pos[3]           = texTrans * glm::vec4(pos[3].x, pos[3].y, 0.f, 0.f);

    vertex vertices[6] = { { pos[0],
                             color,
                             glm::vec2{ texCoords.x, texCoords.y } },
                           { pos[1],
                             color,
                             glm::vec2{ texCoords.x + texCoords.z, texCoords.y } },
                           { pos[2],
                             color,
                             glm::vec2{ texCoords.x + texCoords.z,
                                        texCoords.y + texCoords.w } },
                           { pos[2],
                             color,
                             glm::vec2{ texCoords.x + texCoords.z,
                                        texCoords.y + texCoords.w } },
                           { pos[3],
                             color,
                             glm::vec2{ texCoords.x, texCoords.y + texCoords.w } },
                           { pos[0],
                             color,
                             glm::vec2{ texCoords.x, texCoords.y } } };

    vao->bind();
    vao->get_vbo()->add_subdata(usedVertices * sizeof(vertex),
                                sizeof(vertices), vertices);
    vao->unbind();
    usedVertices += 6;
}
void batch_renderer::batch::set_texture(resource<texture> tex)
{
    _texture = tex;
}
bool batch_renderer::batch::is_texture(resource<texture> other) const
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
void batch_renderer::batch::flush(const glm::mat4& viewProj, resource<shader> textureShader, ptr<rendering_context> context)
{
    if(usedVertices == 0) return;
    textureShader->use();
    textureShader->set_uniform("viewProj", viewProj);
    _texture->bind();
    vao->bind();
    context->draw_arrays(GL_TRIANGLES, 0, usedVertices);
    vao->unbind();
    _texture->unbind();
    usedVertices = 0;
}
batch_renderer::batch_renderer():
    textureShader{ services::get<resource_manager>()->load<shader>(
        string_id("TextureShader"), "/home/matzix/proj/river/src/renderer/shaders/texture_shader.vs",
        "/home/matzix/proj/river/src/renderer/shaders/texture_shader.fs") },
    batches()
{
}
batch_renderer::~batch_renderer()
{
    services::get<resource_manager>()->unload(string_id("TextureShader"));
}
void batch_renderer::render_sprite(const glm::mat4& viewProj, ptr<rendering_context> context,
                                   sprite_renderer& sComponent, transform& tComponent)
{
    static ptr<batch> biggestBatch = &batches.front();
    ptr<batch> empty{};
    ptr<batch> found{};
    for(auto& b : batches)
    {
        if(b.is_texture(sComponent.text))
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
            found->flush(viewProj, textureShader, context);
    }
    else if(empty)
    {
        empty->set_texture(sComponent.text);
        found = empty;
    }
    else
    {
        biggestBatch->flush(viewProj, textureShader, context);
        biggestBatch->set_texture(sComponent.text);
        found        = biggestBatch;
        biggestBatch = &batches.front();
    }
    found->add_quad(tComponent.model, sComponent.color, sComponent.texCoords);
}
void batch_renderer::end_render(const glm::mat4& viewProj, ptr<rendering_context> context)
{
    for(auto& b : batches)
        b.flush(viewProj, textureShader, context);
}
} // namespace river
