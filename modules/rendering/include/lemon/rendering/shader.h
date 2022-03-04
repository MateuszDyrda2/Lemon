#pragma once

#include <lemon/assets/resource.h>
#include <lemon/core/assert.h>
#include <lemon/core/defines.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace lemon {
class LEMON_PUBLIC shader : public resource
{
  public:
    shader(string_id name, const std::string& shaderPath);
    shader(string_id name, const std::vector<byte>& buffer);
    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;
    shader(shader&& other) noexcept;
    shader& operator=(shader&& other) noexcept;
    ~shader();
    void use();
    void set_uniform(const char* name, f32 v1);
    void set_uniform(const char* name, f32 v1, f32 v2);
    void set_uniform(const char* name, f32 v1, f32 v2, f32 v3);
    void set_uniform(const char* name, f32 v1, f32 v2, f32 v3, f32 v4);
    void set_uniform(const char* name, i32 v1);
    void set_uniform(const char* name, i32 v1, i32 v2);
    void set_uniform(const char* name, i32 v1, i32 v2, i32 v3);
    void set_uniform(const char* name, i32 v1, i32 v2, i32 v3, i32 v4);
    void set_uniform(const char* name, u32 v1);
    void set_uniform(const char* name, u32 v1, u32 v2);
    void set_uniform(const char* name, u32 v1, u32 v2, u32 v3);
    void set_uniform(const char* name, u32 v1, u32 v2, u32 v3, u32 v4);
    void set_uniform(const char* name, const vec2& value);
    void set_uniform(const char* name, const ivec2& value);
    void set_uniform(const char* name, const uvec2& value);
    void set_uniform(const char* name, const vec3& value);
    void set_uniform(const char* name, const ivec3& value);
    void set_uniform(const char* name, const uvec3& value);
    void set_uniform(const char* name, const vec4& value);
    void set_uniform(const char* name, const ivec4& value);
    void set_uniform(const char* name, const uvec4& value);
    void set_uniform(const char* name, const mat4& value);

    static string_id get_mocked() { return string_id("mock_shader"); }

  private:
    unsigned int ID;
};
} // namespace lemon
