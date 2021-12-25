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
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &noc, 0);
    if(data)
    {
        width        = w;
        height       = h;
        nrOfChannels = noc;

        glBindTexture(GL_TEXTURE_2D, handle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOG_ERROR("Failed to load texture: %s from %s", name, path);
    }
    stbi_image_free(data);
}
texture::~texture()
{
    glDeleteTextures(1, &handle);
}
} // namespace river
