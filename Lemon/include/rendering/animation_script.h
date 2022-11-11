#pragma once

#include "core/hash_string.h"
#include "scripting/scripting_engine.h"
#include <assets/resource.h>
#include <core/lemon_types.h>

#include <functional>
#include <vector>

namespace lemon {
class LEMON_API animation_script : public resource
{
  public:
    animation_script(hash_str nameid, const std::string& path, scripting_engine& _scriptingEngine);
    virtual ~animation_script();

    animation_script(const animation_script&)            = delete;
    animation_script& operator=(const animation_script&) = delete;
    animation_script(animation_script&& other) noexcept;
    animation_script& operator=(animation_script&& other) noexcept;

    i32 execute(hash_str nameid, f32 value);

    const std::string& get_path() const { return path; }

    static hash_str get_mocked() { return hash_string("mock_animation"); }

  private:
    std::string path;
    std::string name;
    std::unordered_map<hash_str, std::function<i32(f32)>> animations;
};
}