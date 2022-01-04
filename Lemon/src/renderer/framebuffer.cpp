#include <lemon/renderer/framebuffer.h>

namespace lemon {
framebuffer::framebuffer(const glm::ivec2& size):
    size(size)
{
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y,
                 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_ERROR("Failed to create the framebuffer");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
framebuffer::~framebuffer()
{
    glDeleteFramebuffers(1, &handle);
    glDeleteTextures(1, &texture);
}
void framebuffer::bind()
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
}
void framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void framebuffer::resize(const glm::ivec2& newSize)
{
    size = newSize;
    glDeleteFramebuffers(1, &handle);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y,
                 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace lemon
