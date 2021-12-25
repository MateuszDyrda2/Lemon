#include <river/renderer/shader.h>

#include <river/core/logger.h>

#include <fstream>
#include <sstream>
#include <string>

namespace river {
shader::shader(const std::string& vertexPath, const std::string& fragmentPath):
    object("shader")
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure& e)
    {
        // shader read file error
        LOG_ERROR("Shader read failure! %s or %s", vertexPath, fragmentPath);
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

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
void shader::use()
{
    glUseProgram(ID);
}
} // namespace river
