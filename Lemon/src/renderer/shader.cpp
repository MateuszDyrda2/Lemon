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
shader::shader(string_id name, const std::vector<byte>& buffer):
    object(name)
{
    const char* vShaderCode[2] = { "#version 330\n#define VERTEX_PROGRAM\n",
                                   reinterpret_cast<const char*>(buffer.data()) };
    const char* fShaderCode[2] = { "#version 330\n#define FRAGMENT_PROGRAM\n",
                                   reinterpret_cast<const char*>(buffer.data()) };

    // compile shaders
    int success;
    char infoLog[512];

    // vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 2, vShaderCode, NULL);
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
    glShaderSource(fragment, 2, fShaderCode, NULL);
    glCompileShader(fragment);

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
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
void shader::set_uniform(const char* name, f32 v1)
{
    glUniform1f(glGetUniformLocation(ID, name), v1);
}
void shader::set_uniform(const char* name, f32 v1, f32 v2)
{
    glUniform2f(glGetUniformLocation(ID, name), v1, v2);
}
void shader::set_uniform(const char* name, f32 v1, f32 v2, f32 v3)
{
    glUniform3f(glGetUniformLocation(ID, name), v1, v2, v3);
}
void shader::set_uniform(const char* name, f32 v1, f32 v2, f32 v3, f32 v4)
{
    glUniform4f(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
void shader::set_uniform(const char* name, i32 v1)
{
    glUniform1i(glGetUniformLocation(ID, name), v1);
}
void shader::set_uniform(const char* name, i32 v1, i32 v2)
{
    glUniform2i(glGetUniformLocation(ID, name), v1, v2);
}
void shader::set_uniform(const char* name, i32 v1, i32 v2, i32 v3)
{
    glUniform3i(glGetUniformLocation(ID, name), v1, v2, v3);
}
void shader::set_uniform(const char* name, i32 v1, i32 v2, i32 v3, i32 v4)
{
    glUniform4i(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
void shader::set_uniform(const char* name, u32 v1)
{
    glUniform1ui(glGetUniformLocation(ID, name), v1);
}
void shader::set_uniform(const char* name, u32 v1, u32 v2)
{
    glUniform2ui(glGetUniformLocation(ID, name), v1, v2);
}
void shader::set_uniform(const char* name, u32 v1, u32 v2, u32 v3)
{
    glUniform3ui(glGetUniformLocation(ID, name), v1, v2, v3);
}
void shader::set_uniform(const char* name, u32 v1, u32 v2, u32 v3, u32 v4)
{
    glUniform4ui(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
void shader::set_uniform(const char* name, const vec2& value)
{
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const ivec2& value)
{
    glUniform2iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const uvec2& value)
{
    glUniform2uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const vec3& value)
{
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const ivec3& value)
{
    glUniform3iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const uvec3& value)
{
    glUniform3uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const vec4& value)
{
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const ivec4& value)
{
    glUniform4iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const uvec4& value)
{
    glUniform4uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
void shader::set_uniform(const char* name, const mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

} // namespace lemon
