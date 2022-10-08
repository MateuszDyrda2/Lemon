#include <rendering/shader.h>

#include <fstream>
#include <sstream>

#include <rendering/gl_errors.h>

namespace lemon {
using namespace std;
shader::shader(hash_str name, const std::string& shaderPath):
    resource(name)
{
    string shaderCode;
    ifstream shaderFile(shaderPath, ios::binary);

    if(!shaderFile)
    {
        logger::error("Shader read failure: {}", shaderPath);
        return;
    }
    shaderFile.open(shaderPath);
    std::stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderCode = shaderStream.str();

    const char* vShaderCode[2] = { "#version 330\n#define VERTEX_PROGRAM\n",
                                   shaderCode.c_str() };
    const char* fShaderCode[2] = { "#version 330\n#define FRAGMENT_PROGRAM\n",
                                   shaderCode.c_str() };

    // compile shaders
    int success;
    char infoLog[512];

    // vertex shader
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK(glShaderSource(vertex, 2, vShaderCode, NULL));
    GL_CHECK(glCompileShader(vertex));

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        logger::error("Vertex shader compilation failure!: {}", infoLog);
    }
    // fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK(glShaderSource(fragment, 2, fShaderCode, NULL));
    GL_CHECK(glCompileShader(fragment));

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        logger::error("Fragment shader compilation failure!: {}", infoLog);
    }
    ID = glCreateProgram();
    GL_CHECK(glAttachShader(ID, vertex));
    GL_CHECK(glAttachShader(ID, fragment));
    GL_CHECK(glLinkProgram(ID));

    // print errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        logger::error("Shader program linking failure!: {}", infoLog);
    }
    glDetachShader(ID, vertex);
    glDetachShader(ID, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
shader::shader(hash_str name, const std::vector<byte>& buffer):
    resource(name)
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
    GL_CHECK(glShaderSource(vertex, 2, vShaderCode, NULL));
    GL_CHECK(glCompileShader(vertex));

    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        logger::error("Vertex shader compilation failure!: {}", infoLog);
    }
    // fragment shader
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK(glShaderSource(fragment, 2, fShaderCode, NULL));
    GL_CHECK(glCompileShader(fragment));

    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        logger::error("Fragment shader compilation failure!: {}", infoLog);
    }
    ID = glCreateProgram();
    GL_CHECK(glAttachShader(ID, vertex));
    GL_CHECK(glAttachShader(ID, fragment));
    GL_CHECK(glLinkProgram(ID));

    // print errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(success == GL_FALSE)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        logger::error("Shader program linking failure!: {}", infoLog);
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
    resource(std::move(other)), ID(other.ID)
{
    other.ID = 0;
}
shader& shader::operator=(shader&& other) noexcept
{
    if(this != &other)
    {
        resource::operator=(std::move(other));
        std::swap(ID, other.ID);
    }
    return *this;
}
void shader::use()
{
    GL_CHECK(glUseProgram(ID));
}
void shader::set_uniform(const char* name, f32 v1)
{
    GL_CHECK(glUniform1f(glGetUniformLocation(ID, name), v1));
}
void shader::set_uniform(const char* name, f32 v1, f32 v2)
{
    GL_CHECK(glUniform2f(glGetUniformLocation(ID, name), v1, v2));
}
void shader::set_uniform(const char* name, f32 v1, f32 v2, f32 v3)
{
    GL_CHECK(glUniform3f(glGetUniformLocation(ID, name), v1, v2, v3));
}
void shader::set_uniform(const char* name, f32 v1, f32 v2, f32 v3, f32 v4)
{
    GL_CHECK(glUniform4f(glGetUniformLocation(ID, name), v1, v2, v3, v4));
}
void shader::set_uniform(const char* name, i32 v1)
{
    GL_CHECK(glUniform1i(glGetUniformLocation(ID, name), v1));
}
void shader::set_uniform(const char* name, i32 v1, i32 v2)
{
    GL_CHECK(glUniform2i(glGetUniformLocation(ID, name), v1, v2));
}
void shader::set_uniform(const char* name, i32 v1, i32 v2, i32 v3)
{
    GL_CHECK(glUniform3i(glGetUniformLocation(ID, name), v1, v2, v3));
}
void shader::set_uniform(const char* name, i32 v1, i32 v2, i32 v3, i32 v4)
{
    GL_CHECK(glUniform4i(glGetUniformLocation(ID, name), v1, v2, v3, v4));
}
void shader::set_uniform(const char* name, u32 v1)
{
    GL_CHECK(glUniform1ui(glGetUniformLocation(ID, name), v1));
}
void shader::set_uniform(const char* name, u32 v1, u32 v2)
{
    GL_CHECK(glUniform2ui(glGetUniformLocation(ID, name), v1, v2));
}
void shader::set_uniform(const char* name, u32 v1, u32 v2, u32 v3)
{
    GL_CHECK(glUniform3ui(glGetUniformLocation(ID, name), v1, v2, v3));
}
void shader::set_uniform(const char* name, u32 v1, u32 v2, u32 v3, u32 v4)
{
    GL_CHECK(glUniform4ui(glGetUniformLocation(ID, name), v1, v2, v3, v4));
}
void shader::set_uniform(const char* name, const vec2& value)
{
    GL_CHECK(glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const ivec2& value)
{
    GL_CHECK(glUniform2iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const uvec2& value)
{
    GL_CHECK(glUniform2uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const vec3& value)
{
    GL_CHECK(glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const ivec3& value)
{
    GL_CHECK(glUniform3iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const uvec3& value)
{
    GL_CHECK(glUniform3uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const vec4& value)
{
    GL_CHECK(glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const ivec4& value)
{
    GL_CHECK(glUniform4iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const uvec4& value)
{
    GL_CHECK(glUniform4uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value)));
}
void shader::set_uniform(const char* name, const mat4& value)
{
    GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value)));
}
} // namespace lemon
