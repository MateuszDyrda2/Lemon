#include <scripting/scripting_engine.h>

extern "C"
{
#include <lauxlib.h>
#include <luajit.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include <fstream>

namespace lemon {
using namespace luabridge;
using namespace std;
#define LUA_CHECK(_func, L, ...)                             \
    int err = _func(L, __VA_ARGS__);                         \
    if (err != 0)                                            \
    {                                                        \
        logger::error("Lua error: {}", lua_tostring(L, 01)); \
        lua_pop(L, 1);                                       \
    }

scripting_engine::scripting_engine()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

scripting_engine::~scripting_engine() { }

void scripting_engine::load_file(const std::string& path)
{
    LUA_CHECK(luaL_dofile, L, path.c_str());
}
}
