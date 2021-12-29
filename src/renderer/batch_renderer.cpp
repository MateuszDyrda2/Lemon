#include <river/renderer/batch_renderer.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <river/renderer/texture.h>

#include <glad/glad.h>

namespace river {
struct vertex
{
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoords;
};
struct batch
{
    size_type maxVertices;
    size_type usedVertices;
    u32 vao, vbo;
    ptr<texture> _texture;

    batch(size_type maxVertices):
        maxVertices(maxVertices),
        usedVertices{}, vao{}, vbo{}, _texture{ nullptr }
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, maxVertices * sizeof(vertex),
                     nullptr, GL_STREAM_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                              sizeof(vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                              sizeof(vertex), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                              sizeof(vertex), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    string_id get_texture_id() const noexcept
    {
        return _texture->get_id();
    }
    bool is_empty() const noexcept { return usedVertices == 0ULL; }
    bool is_room_for(size_type vertNum) const noexcept
    {
        return maxVertices - usedVertices >= vertNum;
    }
    void add(const vertex& v)
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, usedVertices * sizeof(vertex), sizeof(vertex), &v);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        ++usedVertices;
    }
    void set_texture(ptr<texture> tex)
    {
        _texture = tex;
    }
    void render()
    {
        if(usedVertices == 0) return;
    }
};
} // namespace river
