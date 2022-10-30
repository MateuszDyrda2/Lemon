#pragma once

#include <core/defines.h>
class lua_State;
namespace lemon {
class LEMON_API scripting_engine
{
  public:
    scripting_engine();
    ~scripting_engine();

    void load_file(const std::string& path);
    void get_animation_func();

  private:
    lua_State* L;
};
}
