#pragma once

#include <lemon/assets/resource.h>
#include <lemon/core/assert.h>
#include <lemon/core/lemon_types.h>
#include <lemon/core/math/mat4.h>
#include <lemon/core/math/vec2.h>
#include <lemon/core/math/vec3.h>
#include <lemon/core/math/vec4.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

namespace lemon {
class shader : public resource
{
  public:
    shader(hash_str name, const std::string& shaderPath);
    shader(hash_str name, const std::vector<byte>& buffer);
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

    static hash_str get_mocked() { return hash_string("mock_shader"); }

  private:
    unsigned int ID;
};
} // namespace lemon
