#include "scripting/script_entity.h"
#include <scripting/scripting_engine.h>

extern "C"
{
#include <lauxlib.h>
#include <luajit.h>
#include <lualib.h>
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include <LuaBridge/LuaBridge.h>
#pragma GCC diagnostic pop

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

std::unordered_map<hash_str, function<i32(f32)>> get_key_value_map(lua_State* L, const LuaRef& table)
{
    std::unordered_map<hash_str, function<i32(f32)>> result;
    if (table.isNil())
    {
        logger::error("Lua table is nil");
        return result;
    }

    if (!push(L, table)) return result;

    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        if (lua_isstring(L, -2))
        {
            auto f = LuaRef::fromStack(L, -1);
            if (f.isFunction())
            {
                result.emplace(hash_string_d(lua_tostring(L, -2)), [f = std::move(f)](f32 v) -> i32 {
                    auto res = f(v);
                    if (res.hasFailed()) logger::error("Invokation of a lua animation script raised an error {}", res.errorMessage());

                    auto value = res[0];
                    if (!value.isNumber()) logger::error("The animation function should return a number");

                    return value.cast<i32>().value();
                });
            }
        }
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return result;
}

scripting_engine::scripting_engine()
{
    L = luaL_newstate();
    registerMainThread(L);
    luaL_openlibs(L);

    getGlobalNamespace(L)
        .beginNamespace("lemon")
        .beginClass<logger>("logger")
        .addStaticFunction("info", &logger::info_c)
        .addStaticFunction("warn", &logger::warn_c)
        .addStaticFunction("error", &logger::error_c)
        .addStaticFunction("fatal", &logger::fatal_c)
        .endClass()
        .beginClass<script_message_proxy>("ent_proxy")
        .addFunction("arg",
                     overload<i32>(&script_message_proxy::arg),
                     overload<f32>(&script_message_proxy::arg),
                     overload<bool>(&script_message_proxy::arg),
                     overload<vec2>(&script_message_proxy::arg),
                     overload<const char*>(&script_message_proxy::arg))
        .addFunction("push", &script_message_proxy::push)
        .endClass()
        .beginClass<script_animator>("animator")
        .addFunction("start", &script_animator::start_animation)
        .addFunction("stop", &script_animator::stop_animations)
        .endClass()
        .beginClass<script_entity>("entity")
        .addProperty("handle", &script_entity::handle)
        .addProperty("animator", &script_entity::get_animator)
        .addFunction("message", &script_entity::create_message)
        .endClass()
        .endNamespace();
}

scripting_engine::~scripting_engine()
{
    lua_close(L);
}

void scripting_engine::load_file(const std::string& path)
{
    LUA_CHECK(luaL_dofile, L, path.c_str()); // load the file
}

unordered_map<hash_str, function<i32(f32)>> scripting_engine::get_animation(const std::string& name)
{
    auto table = getGlobal(L, name.c_str());
    return get_key_value_map(L, table);
}

void scripting_engine::call_function(const std::string& scope, const std::string& func)
{
    auto table = getGlobal(L, scope.c_str());
    table[func.c_str()];
}
}
