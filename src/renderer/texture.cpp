#include <river/renderer/texture.h>

#include <river/core/logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace river {
texture::texture(const std::string& name, const std::string& path):
    object(name)
{
    int w, h, noc;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    // set the texture wrapping / filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &noc, 0);
    if(data)
    {
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture: %s from %s", name, path);
    }
    stbi_image_free(data);
}
void texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, handle);
}
texture::~texture()
{
    glDeleteTextures(1, &handle);
}
} // namespace river
