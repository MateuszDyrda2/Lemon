#include <river/renderer/texture.h>

#include <river/core/logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace river {
texture::texture(string_id name, const std::string& path):
    object(name)
{
    int w, h, noc;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &noc, 0);
    if(data)
    {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        // set the texture wrapping / filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load and generate the texture
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: %s from %s", name.get_string().c_str(), path.c_str());
    }
}
void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);
}
void texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
texture::~texture()
{
    glDeleteTextures(1, &handle);
}
texture::texture(texture&& other) noexcept:
    object(other), handle(other.handle)
{
    other.handle = 0;
}
texture& texture::operator=(texture&& other) noexcept
{
    if(this != &other)
    {
        object::operator=(std::move(other));
        std::swap(handle, other.handle);
    }
    return *this;
}
} // namespace river
