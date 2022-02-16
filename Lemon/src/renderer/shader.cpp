#include <lemon/renderer/shader.h>

#include <fstream>
#include <sstream>

namespace lemon {
shader::shader(string_id name, const std::string& shaderPath):
    object(name)
{
    std::string shaderCode;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(LEMON_RESOURCE_PATH + shaderPath);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();

        shaderFile.close();

        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure& e)
    {
        LOG_ERROR("Shader read failure! %s",
                  shaderPath.c_str());
    }

    std::string vsDefines   = "#version 330\n#define VERTEX_PROGRAM\n";
    std::string fsDefines   = "#version 330\n#define FRAGMENT_PROGRAM\n";
    const char* vShaderCode = (vsDefines + shaderCode).c_str();
    const char* fShaderCode = (fsDefines + shaderCode).c_str();

    // compile shaders
    int success;
    char infoLog[512];

    // vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        LOG_ERROR("Vertex shader compilation failure!: %s", infoLog);
    }
    // fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        LOG_ERROR("Fragment shader compilation failure!: %s", infoLog);
    }
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        LOG_ERROR("Shader program linking failure!: %s", infoLog);
    }
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
shader::~shader()
{
    if(ID) glDeleteProgram(ID);
}
shader::shader(shader&& other) noexcept:
    object(other), ID(other.ID)
{
    other.ID = 0;
}
shader& shader::operator=(shader&& other) noexcept
{
    if(this != &other)
    {
        object::operator=(std::move(other));
        std::swap(ID, other.ID);
    }
    return *this;
}
void shader::use()
{
    glUseProgram(ID);
}
} // namespace lemon
