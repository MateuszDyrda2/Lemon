#include <rendering/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <rendering/gl_errors.h>

namespace lemon {
texture::texture(hashstr name, const std::string& path,
                 wrap wrapping, filter filtering):
    resource(name),
    handle(0),
    wrapping(wrapping), filtering(filtering)
{
    int width;
    int height;
    int numberOfChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(
        path.c_str(),
        &width,
        &height,
        &numberOfChannels,
        0);
    if (!data)
    {
        logger::error("Failed to load texture from {}", path);
        return;
    }
    size         = { width, height };
    nrOfChannels = numberOfChannels;

    glGenTextures(1, &handle);
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapping)));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapping)));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filtering)));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filtering)));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto glColor = numberOfChannels == 4 ? GL_RGBA : GL_RGB;
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, glColor, size.x, size.y, 0, glColor, GL_UNSIGNED_BYTE, data));

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    stbi_image_free(data);
}
texture::texture(hashstr name, const std::vector<u8>& buffer):
    resource(name), handle(0),
    wrapping(wrap::clampToBorder), filtering(filter::nearest)
{
    int width;
    int height;
    int numberOfChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load_from_memory(
        buffer.data(),
        static_cast<int>(buffer.size()),
        &width,
        &height,
        &numberOfChannels,
        0);
    if (!data)
    {
        logger::error("Failed to load texture from buffer");
        return;
    }
    size         = { width, height };
    nrOfChannels = numberOfChannels;

    glGenTextures(1, &handle);
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    auto glColor = numberOfChannels == 4 ? GL_RGBA : GL_RGB;
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, glColor, size.x, size.y, 0, glColor, GL_UNSIGNED_BYTE, data));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    stbi_image_free(data);
}
texture::texture(hashstr name, const std::string& path):
    texture(name, path, wrap::clampToBorder, filter::nearest)
{ }
texture::texture(hashstr name, const ivec2& size, const color& c):
    texture(name, size, c, wrap::clampToBorder, filter::nearest)
{ }
texture::texture(hashstr name, const ivec2& size, const color& c, wrap wrapping, filter filtering):
    resource(name), size(size), wrapping(wrapping), filtering(filtering)
{
    lemon_assert(!(size.x % 4) && !(size.y % 4));
    std::vector<unsigned char> data(size.x * size.y * 4);
    auto&& [r, g, b, a] = c.to_u8();
    for (std::size_t i = 0; i < data.size(); i += 4)
    {
        data[i]     = r;
        data[i + 1] = g;
        data[i + 2] = b;
        data[i + 3] = a;
    }
    glGenTextures(1, &handle);
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(wrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(wrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(filtering));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(filtering));
    GL_CHECK(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        size.x,
        size.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data.data()));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
void texture::bind() const
{
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, handle));
}
void texture::unbind() const
{
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
texture::~texture()
{
    if (!handle) return;

    glDeleteTextures(1, &handle);
}
texture::texture(texture&& other) noexcept:
    resource(std::move(other)), handle(other.handle)
{
    other.handle = 0;
}
texture& texture::operator=(texture&& other) noexcept
{
    if (this != &other)
    {
        resource::operator=(std::move(other));
        std::swap(handle, other.handle);
    }
    return *this;
}
} // namespace lemon
