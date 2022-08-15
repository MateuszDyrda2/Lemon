#include <rendering/texture.h>

#include <core/logger.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glad/glad.h>

namespace lemon {
texture::texture(hash_str name, const std::string& path,
                 wrap wrapping, filter filtering):
    resource(name),
    handle(0),
    wrapping(wrapping), filtering(filtering)
{
    int w, h, noc;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &noc, 0);
    if(data)
    {
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(handle, GL_TEXTURE_WRAP_S, static_cast<int>(wrapping));
        glTexParameteri(handle, GL_TEXTURE_WRAP_T, static_cast<int>(wrapping));
        glTexParameteri(handle, GL_TEXTURE_MIN_FILTER, static_cast<int>(filtering));
        glTexParameteri(handle, GL_TEXTURE_MAG_FILTER, static_cast<int>(filtering));
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        if(noc == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        else if(noc == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else
    {
        logger::error("Failed to load texture from {}", path);
    }
}
texture::texture(hash_str name, const std::vector<u8>& buffer):
    resource(name), handle(0),
    wrapping(wrap::clampToBorder), filtering(filter::nearest)
{
    int w /* width */,
        h /* height */,
        noc /* number of channels */;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load_from_memory(buffer.data(), static_cast<int>(buffer.size()), &w, &h, &noc, 0);
    if(data)
    {
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        glTexParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        if(noc == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        else if(noc == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
    else
    {
        logger::error("Failed to load texture: from buffer");
    }
}
texture::texture(hash_str name, const std::string& path):
    texture(name, path, wrap::clampToBorder, filter::nearest)
{ }
texture::texture(hash_str name, const ivec2& size, const color& c):
    texture(name, size, c, wrap::clampToBorder, filter::nearest)
{ }
texture::texture(hash_str name, const ivec2& size, const color& c, wrap wrapping, filter filtering):
    resource(name), size(size), wrapping(wrapping), filtering(filtering)
{
    lemon_assert(!(size.x % 4) && !(size.y % 4));
    std::vector<unsigned char> data(size.x * size.y * 4);
    auto&& [r, g, b, a] = c.to_u8();
    for(std::size_t i = 0; i < data.size(); i += 4)
    {
        data[i]     = r;
        data[i + 1] = g;
        data[i + 2] = b;
        data[i + 3] = a;
    }
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filtering));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filtering));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}
void texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, handle);
}
void texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
texture::~texture()
{
    if(handle)
        glDeleteTextures(1, &handle);
}
texture::texture(texture&& other) noexcept:
    resource(std::move(other)), handle(other.handle)
{
    other.handle = 0;
}
texture& texture::operator=(texture&& other) noexcept
{
    if(this != &other)
    {
        resource::operator=(std::move(other));
        std::swap(handle, other.handle);
    }
    return *this;
}
} // namespace lemon
