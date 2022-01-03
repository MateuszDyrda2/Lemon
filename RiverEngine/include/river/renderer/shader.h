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
    shader(string_id name, const std::string& vertexPath, const std::string& fragmentPath);
    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;
    shader(shader&& other) noexcept;
    shader& operator=(shader&& other) noexcept;
    ~shader();
    void use();
    template<class T>
    void set_uniform(const char* name, const T& arg) { LOG_ERROR("Wrong uniform type set"); }

    template<class T>
    void set_uniform(const char* name, const T& arg1, const T& arg2) { LOG_ERROR("Wrong uniform type set"); }
    template<class T>
    void set_uniform(const char* name, const T& arg1, const T& arg2, const T& arg3) { LOG_ERROR("Wrong uniform type set"); }
    template<class T>
    void set_uniform(const char* name, const T& arg1, const T& arg2, const T& arg3, const T& arg4)
    {
        LOG_ERROR("Wrong uniform type set");
    }

  private:
    unsigned int ID;
};
template<>
inline void shader::set_uniform<f32>(const char* name, const f32& value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}
template<>
inline void shader::set_uniform<f32>(const char* name, const f32& v1, const f32& v2)
{
    glUniform2f(glGetUniformLocation(ID, name), v1, v2);
}
template<>
inline void shader::set_uniform<f32>(const char* name, const f32& v1, const f32& v2, const f32& v3)
{
    glUniform3f(glGetUniformLocation(ID, name), v1, v2, v3);
}
template<>
inline void shader::set_uniform<f32>(const char* name,
                                     const f32& v1, const f32& v2, const f32& v3, const f32& v4)
{
    glUniform4f(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<i32>(const char* name, const i32& value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}
template<>
inline void shader::set_uniform<i32>(const char* name, const i32& v1, const i32& v2)
{
    glUniform2i(glGetUniformLocation(ID, name), v1, v2);
}
template<>
inline void shader::set_uniform<i32>(const char* name, const i32& v1, const i32& v2, const i32& v3)
{
    glUniform3i(glGetUniformLocation(ID, name), v1, v2, v3);
}
template<>
inline void shader::set_uniform<i32>(const char* name,
                                     const i32& v1, const i32& v2, const i32& v3, const i32& v4)
{
    glUniform4i(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<u32>(const char* name, const u32& value)
{
    glUniform1ui(glGetUniformLocation(ID, name), value);
}
template<>
inline void shader::set_uniform<u32>(const char* name, const u32& v1, const u32& v2)
{
    glUniform2ui(glGetUniformLocation(ID, name), v1, v2);
}
template<>
inline void shader::set_uniform<u32>(const char* name, const u32& v1, const u32& v2, const u32& v3)
{
    glUniform3ui(glGetUniformLocation(ID, name), v1, v2, v3);
}
template<>
inline void shader::set_uniform<u32>(const char* name,
                                     const u32& v1, const u32& v2, const u32& v3, const u32& v4)
{
    glUniform4ui(glGetUniformLocation(ID, name), v1, v2, v3, v4);
}
template<>
inline void shader::set_uniform<glm::vec1>(const char* name, const glm::vec1& value)
{
    glUniform1fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec2>(const char* name, const glm::vec2& value)
{
    glUniform2fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec3>(const char* name, const glm::vec3& value)
{
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::vec4>(const char* name, const glm::vec4& value)
{
    glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec1>(const char* name, const glm::ivec1& value)
{
    glUniform1iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec2>(const char* name, const glm::ivec2& value)
{
    glUniform2iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec3>(const char* name, const glm::ivec3& value)
{
    glUniform3iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::ivec4>(const char* name, const glm::ivec4& value)
{
    glUniform4iv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec1>(const char* name, const glm::uvec1& value)
{
    glUniform1uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec2>(const char* name, const glm::uvec2& value)
{
    glUniform2uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec3>(const char* name, const glm::uvec3& value)
{
    glUniform3uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::uvec4>(const char* name, const glm::uvec4& value)
{
    glUniform4uiv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat2>(const char* name, const glm::mat2& value)
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat3>(const char* name, const glm::mat3& value)
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
template<>
inline void shader::set_uniform<glm::mat4>(const char* name, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}
} // namespace river
