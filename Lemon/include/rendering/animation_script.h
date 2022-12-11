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
    animation_script(hashstr nameid, const std::string& path, scripting_engine& _scriptingEngine);
    virtual ~animation_script();

    animation_script(const animation_script&)            = delete;
    animation_script& operator=(const animation_script&) = delete;
    animation_script(animation_script&& other) noexcept;
    animation_script& operator=(animation_script&& other) noexcept;

    i32 execute(hashstr nameid, f32 value);

    const std::string& get_path() const { return path; }

    static hashstr get_mocked() { return "mock_animation"_hs; }

  private:
    std::string path;
    std::string name;
    std::unordered_map<hashstr, std::function<i32(f32)>> animations;
};
}
