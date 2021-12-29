#include <river/renderer/renderer.h>

#include <river/core/assert.h>
#include <river/core/logger.h>

#include <glm/gtx/transform.hpp>
#include <river/game/scene.h>
#include <river/window/window.h>

namespace river {
renderer::renderer(window* w)
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // failed to initialize glad
        LOG_FATAL("Failed to initialize glad");
    }

    framebufferSize = event_handler::subscribe<size_type, size_type>("FramebufferSize", [](size_type width, size_type height) {
        LOG_MESSAGE("Framebuffer resize: %dx%d", width, height);
        glViewport(0, 0, width, height);
    });
    glViewport(0, 0, w->get_width(), w->get_height());
}
renderer::~renderer()
{
    framebufferSize.dispose();
}
void renderer::render(ptr<scene> currentScene)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void renderer::draw_sprite(texture& tex, transform& trans)
{
    textureShader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model           = glm::translate(model, trans.get_position());
    model           = glm::translate(model, glm::vec3(0.5f * trans.get_scale().x,
                                                      0.5f * trans.get_scale().y, 0.0f));
    model           = glm::rotate(model,
                                  glm::radians(trans.get_rotation().z),
                                  glm::vec3(0.0f, 0.0f, 1.0f));
    model           = glm::translate(model, glm::vec3(-0.5f * trans.get_scale().x,
                                                      -0.5f * trans.get_scale().y, 0.0f));

    model = glm::scale(model, trans.get_scale());

    textureShader->set_uniform("model", model);
}
void renderer::init_renderer_data()
{
    textureShader = std::make_unique<shader>(
        "../../src/renderer/shaders/texture_shader.vs",
        "../../src/renderer/shaders/texture_shader.fs");
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(GL_ARRAY_BUFFER, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
} // namespace river
