#pragma once

#include <core/defines.h>
#include <core/hash_string.h>
#include <core/lemon_types.h>

#include <functional>
#include <string>
#include <unordered_map>

class lua_State;
namespace lemon {
class LEMON_API scripting_engine
{
  public:
    scripting_engine();
    ~scripting_engine();

    void load_file(const std::string& path);
    std::unordered_map<hashstr, std::function<i32(f32)>> get_animation(const std::string& name);
    void call_function(const std::string& scope, const std::string& func);
    lua_State* get_state() { return L; }

  private:
    lua_State* L;
};
}
