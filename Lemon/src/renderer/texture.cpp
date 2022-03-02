#include <lemon/renderer/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>

namespace lemon {
texture::texture(string_id name, const std::string& path):
    object(name), handle(nullptr)
{
    int w, h, noc;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load((LEMON_RESOURCE_PATH + path).c_str(), &w, &h, &noc, 0);
    if(data)
    {
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        handle = rendering_context::generate_texture()
                     ->bind()
                     ->set_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
                     ->set_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
                     ->set_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
                     ->set_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
                     ->set_unpack_alignment(1);
        if(noc == 3)
            handle->create_image(0, GL_RGB, size, GL_RGB, GL_UNSIGNED_BYTE, data);

        else if(noc == 4)
        {
            handle->create_image(0, GL_RGBA, size, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        handle->unbind();
        stbi_image_free(data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: %s from %s", name.get_string(), path.c_str());
    }
}
texture::texture(string_id name, const std::vector<byte>& buffer):
    object(name), handle(nullptr)
{
    int w /* width */,
        h /* height */,
        noc /* number of channels */;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load_from_memory(buffer.data(), buffer.size(), &w, &h, &noc, 0);
    if(data)
    {
        size.x       = w;
        size.y       = h;
        nrOfChannels = noc;

        handle = rendering_context::generate_texture()
                     ->bind()
                     ->set_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER)
                     ->set_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER)
                     ->set_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
                     ->set_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST)
                     ->set_unpack_alignment(1);
        if(noc == 3)
            handle->create_image(0, GL_RGB, size, GL_RGB, GL_UNSIGNED_BYTE, data);

        else if(noc == 4)
        {
            handle->create_image(0, GL_RGBA, size, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        handle->unbind();

        stbi_image_free(data);
    }
    else
    {
        LOG_ERROR("Failed to load texture: %s from buffer", name.get_string());
    }
}
void texture::bind() const
{
    handle->bind();
}
void texture::unbind() const
{
    handle->unbind();
}
texture::~texture()
{
    rendering_context::destroy_texture(&handle);
}
texture::texture(texture&& other) noexcept:
    object(other), handle(other.handle)
{
    other.handle = nullptr;
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
} // namespace lemon
