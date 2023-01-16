/** @file shader.h
 * @brief File with a shader definition
 */
#pragma once

#include <assets/resource.h>
#include <core/assert.h>
#include <core/defines.h>
#include <core/lemon_types.h>
#include <core/math/mat4.h>
#include <core/math/vec2.h>
#include <core/math/vec3.h>
#include <core/math/vec4.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace lemon {
/** Class representation of a shader */
class LEMON_API shader : public resource
{
  public:
    /** @brief Creates a shader
     * @param nameid id of the shader
     * @param shaderPath path of the shader resource
     */
    shader(hashstr nameid, const std::string& shaderPath);
    /** @brief Creates a shader
     * @param nameid id of the shader
     * @param buffer shader data
     */
    shader(hashstr nameid, const std::vector<byte>& buffer);
    shader(const shader&)            = delete;
    shader& operator=(const shader&) = delete;
    shader(shader&& other) noexcept;
    shader& operator=(shader&& other) noexcept;
    ~shader();
    /** @brief Binds the shader program */
    void use();
    void set_uniform(const char* name, f32 v1);
    /** @brief Set the uniform */
    void set_uniform(const char* name, f32 v1, f32 v2);
    /** @brief Set the uniform */
    void set_uniform(const char* name, f32 v1, f32 v2, f32 v3);
    /** @brief Set the uniform */
    void set_uniform(const char* name, f32 v1, f32 v2, f32 v3, f32 v4);
    /** @brief Set the uniform */
    void set_uniform(const char* name, i32 v1);
    /** @brief Set the uniform */
    void set_uniform(const char* name, i32 v1, i32 v2);
    /** @brief Set the uniform */
    void set_uniform(const char* name, i32 v1, i32 v2, i32 v3);
    /** @brief Set the uniform */
    void set_uniform(const char* name, i32 v1, i32 v2, i32 v3, i32 v4);
    /** @brief Set the uniform */
    void set_uniform(const char* name, u32 v1);
    /** @brief Set the uniform */
    void set_uniform(const char* name, u32 v1, u32 v2);
    /** @brief Set the uniform */
    void set_uniform(const char* name, u32 v1, u32 v2, u32 v3);
    /** @brief Set the uniform */
    void set_uniform(const char* name, u32 v1, u32 v2, u32 v3, u32 v4);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const vec2& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const ivec2& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const uvec2& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const vec3& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const ivec3& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const uvec3& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const vec4& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const ivec4& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const uvec4& value);
    /** @brief Set the uniform */
    void set_uniform(const char* name, const mat4& value);
    /** @brief Returns the mocked shader id */
    static hashstr get_mocked() { return "mock_shader"_hs; }

  private:
    unsigned int ID;
};
} // namespace lemon
