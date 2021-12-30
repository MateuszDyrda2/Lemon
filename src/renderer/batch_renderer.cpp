#include <river/renderer/batch_renderer.h>

#include <river/renderer/texture.h>
#include <river/renderer/vertex_array.h>

#include <algorithm>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace river {
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

    batch():
        usedVertices{}, vao(create_owned<vertex_array>()), _texture{ nullptr }
    {
        vao->add_vertex_buffer(create_owned<vertex_buffer>(batch_renderer::maxVertices * sizeof(vertex)));
        vao->get_vbo()->enable_vertex_attrib(0, 1, GL_FLOAT_VEC2, false, sizeof(vertex), 0);
        vao->get_vbo()->enable_vertex_attrib(1, 1, GL_FLOAT_VEC4, false, sizeof(vertex), 0);
        vao->get_vbo()->enable_vertex_attrib(2, 1, GL_FLOAT_VEC2, false, sizeof(vertex), 0);
        vao->unbind();
    }
    bool is_empty() const noexcept { return usedVertices == 0ULL; }
    void add_quad(const glm::mat4& trans, glm::vec4 color, glm::vec4 texCoords)
    {
        glm::vec2 pos[4] = { { -0.5f, -0.5f },
                             { 0.5f, -0.5f },
                             { 0.5f, 0.5f },
                             { -0.5f, 0.5f } };

        pos[0] = trans * glm::vec4(pos[0].x, pos[0].y, 0.f, 0.f);
        pos[1] = trans * glm::vec4(pos[1].x, pos[1].y, 0.f, 0.f);
        pos[2] = trans * glm::vec4(pos[2].x, pos[2].y, 0.f, 0.f);
        pos[3] = trans * glm::vec4(pos[3].x, pos[3].y, 0.f, 0.f);

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
    void set_texture(resource<texture> tex)
    {
        _texture = tex;
    }
    bool is_texture(resource<texture> other) const
    {
        return other == _texture;
    }
    ptr<batch> get_bigger(ptr<batch> other) const
    {
        return usedVertices > other->usedVertices ? ptr<batch>(this) : other;
    }
    bool is_full() const
    {
        return batch_renderer::maxVertices - usedVertices < 6;
    }
    void flush(const glm::mat4& viewProj, resource<shader> textureShader, ptr<opengl_context> context)
    {
        if(usedVertices == 0) return;
        textureShader->use();
        textureShader->set_uniform("viewProj", viewProj);
        _texture->bind();
        vao->bind();
        context->draw_arrays(GL_TRIANGLES, 0, usedVertices);
        vao->unbind();
        usedVertices = 0;
    }
};
void batch_renderer::render_sprite(const glm::mat4& viewProj, ptr<opengl_context> context,
                                   sprite_renderer& sComponent, transform& tComponent)
{
    static ptr<batch> biggestBatch = batches.front().get();
    ptr<batch> empty{};
    ptr<batch> found{};
    for(auto& b : batches)
    {
        if(b->is_texture(sComponent.texture))
        {
            found = b.get();
        }
        else if(b->is_empty())
        {
            empty = b.get();
        }
    }
    if(found)
    {
        if(found->is_full())
            found->flush(viewProj, textureShader, context);
    }
    else if(empty)
    {
        empty->set_texture(sComponent.texture);
        found = empty;
    }
    else
    {
        biggestBatch->flush(viewProj, textureShader, context);
        biggestBatch->set_texture(sComponent.texture);
        found        = biggestBatch;
        biggestBatch = batches.front().get();
    }
    found->add_quad(tComponent.model, sComponent.color, sComponent.texCoords);
}
void batch_renderer::end_render(const glm::mat4& viewProj, ptr<opengl_context> context)
{
    for(auto& b : batches)
        b->flush(viewProj, textureShader, context);
}

} // namespace river
