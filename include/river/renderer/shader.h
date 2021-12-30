#pragma once

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <river/core/assert.h>
#include <river/game/object.h>

namespace river {
class shader : public object
{
  public:
    shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    template<class... Args>
    void set_uniform(const char* name, Args&&... args);

  private:
    unsigned int ID;
};

template<class... Args>
inline void shader::set_uniform(const char* name, Args&&... args)
{
    // RIVER_SASSERT(false, "Not implemented");
}
template<>
inline void shader::set_uniform<f32>(const char* name, f32&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1f(location, value);
}
template<>
inline void shader::set_uniform<f32, f32>(const char* name, f32&& v1, f32&& v2)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2f(location, v1, v2);
}
template<>
inline void shader::set_uniform<f32, f32, f32>(const char* name, f32&& v1, f32&& v2, f32&& v3)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3f(location, v1, v2, v3);
}
template<>
inline void shader::set_uniform<f32, f32, f32, f32>(const char* name,
                                                    f32&& v1, f32&& v2, f32&& v3, f32&& v4)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4f(location, v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<i32>(const char* name, i32&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1i(location, value);
}
template<>
inline void shader::set_uniform<i32, i32>(const char* name, i32&& v1, i32&& v2)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2i(location, v1, v2);
}
template<>
inline void shader::set_uniform<i32, i32, i32>(const char* name, i32&& v1, i32&& v2, i32&& v3)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3i(location, v1, v2, v3);
}
template<>
inline void shader::set_uniform<i32, i32, i32, i32>(const char* name,
                                                    i32&& v1, i32&& v2, i32&& v3, i32&& v4)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4i(location, v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<u32>(const char* name, u32&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1ui(location, value);
}
template<>
inline void shader::set_uniform<u32, u32>(const char* name, u32&& v1, u32&& v2)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2ui(location, v1, v2);
}
template<>
inline void shader::set_uniform<u32, u32, u32>(const char* name, u32&& v1, u32&& v2, u32&& v3)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3ui(location, v1, v2, v3);
}
template<>
inline void shader::set_uniform<u32, u32, u32, u32>(const char* name,
                                                    u32&& v1, u32&& v2, u32&& v3, u32&& v4)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4ui(location, v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<glm::vec1>(const char* name, glm::vec1&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1fv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec2>(const char* name, glm::vec2&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2fv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec3>(const char* name, glm::vec3&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec4>(const char* name, glm::vec4&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec1>(const char* name, glm::ivec1&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1iv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec2>(const char* name, glm::ivec2&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2iv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec3>(const char* name, glm::ivec3&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3iv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec4>(const char* name, glm::ivec4&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4iv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec1>(const char* name, glm::uvec1&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform1uiv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec2>(const char* name, glm::uvec2&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform2uiv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec3>(const char* name, glm::uvec3&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform3uiv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec4>(const char* name, glm::uvec4&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniform4uiv(location, 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat2>(const char* name, glm::mat2&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat3>(const char* name, glm::mat3&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat4>(const char* name, glm::mat4&& value)
{
    auto location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
} // namespace river
