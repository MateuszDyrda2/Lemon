#include <scripting/scripting_engine.h>

#include "core/math/mat4.h"
#include "core/math/vec2.h"
#include "core/math/vec3.h"
#include "core/math/vec4.h"
#include "physics/systems/physics_system.h"
#include "platform/input.h"
#include "platform/keycodes.h"
#include "scripting/script_entity.h"
#include "world/systems/transform_system.h"
#include <type_traits>
#include <world/scene.h>

extern "C"
{
#include <lauxlib.h>
#include <lua.h>
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

std::unordered_map<hashstr, function<i32(f32)>> get_key_value_map(lua_State* L, const LuaRef& table)
{
    std::unordered_map<hashstr, function<i32(f32)>> result;
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
                result.emplace(hashstr::runtime_hash(lua_tostring(L, -2)), [f = std::move(f)](f32 v) -> i32 {
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

scripting_engine::scripting_engine(input& _input, event_queue& _eventQueue)
{
    L = luaL_newstate();
    registerMainThread(L);
    luaL_openlibs(L);

    getGlobalNamespace(L)
        .beginNamespace("lemon")
        .beginClass<hashstr>("hashstr")
        .addProperty("str", &hashstr::str)
        .addProperty("value", &hashstr::value)
        .endClass()
        .beginClass<event_queue>("events")
        .addFunction(
            "close_window", +[](event_queue* q) { q->event("CloseWindow"_hs).fire(new event_args); })
        .endClass()
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
        .addFunction(
            "set_dirty", +[](script_entity* ent) {
                ent->_scene->get_entity(entity_t(ent->handle)).set_dirty();
            })
        .addFunction(
            "get_entity", +[](script_entity* ent, const char* name) {
                auto n = ent->_scene->get_entity_registry().get(hashstr::runtime_hash(name));
                return script_entity(ent->_messageBus, ent->_scene, u32(n));
            })
        .addFunction(
            "get_script_entity", +[](script_entity* ent, u32 id) {
                return script_entity(ent->_messageBus, ent->_scene, id);
            })
        .endClass()
        .beginClass<input>("input")
        .addFunction(
            "add_key", +[](input* i, int k, int a, const char* name) { return i->add_key(keycode(k), key_action(a), hashstr::runtime_hash(name)); })
        .addFunction(
            "add_mouse", +[](input* i, int k, int a, const char* name) { return i->add_key(mouse(k), key_action(a), hashstr::runtime_hash(name)); })
        .addFunction(
            "add_gamepad", +[](input* i, int k, int a, const char* name) { return i->add_key(gamepad(k), key_action(a), hashstr::runtime_hash(name)); })
        .addFunction(
            "check_key", +[](input* i, int k, int a) { return i->check_key(keycode(k), key_action(a)); })
        .addFunction(
            "check_mouse", +[](input* i, int k, int a) { return i->check_key(mouse(k), key_action(a)); })
        .addFunction(
            "check_gamepad", +[](input* i, int k, int a) { return i->check_key(gamepad(k), key_action(a)); })
        .addFunction(
            "check_gamepad_axis", +[](input* i, int k) { return i->check_axis(gamepad_axis(k)); })
        .addFunction(
            "check_mouse_axis", +[](input* i, int k) { return i->check_axis(mouse_axis(k)); })
        .addFunction(
            "check_axis", +[](input* i, int a, int b) { return i->check_axis({ keycode(a), keycode(b) }); })
        .endClass()
        .beginClass<physics_system>("physics")
        .addStaticFunction(
            "set_velocity", +[](script_entity* ent, f32 x, f32 y) { physics_system::set_velocity(entity(*ent->_scene, entity_t(ent->handle)), { x, y }); })
        .addStaticFunction(
            "add_velocity", +[](script_entity* ent, f32 x, f32 y) { physics_system::add_velocity(entity(*ent->_scene, entity_t(ent->handle)), { x, y }); })
        .addStaticFunction(
            "move_position", +[](script_entity* ent, vec2 newPosition, f32 delta) {
                physics_system::move_position(entity(*ent->_scene, entity_t(ent->handle)), newPosition, delta);
            })
        .endClass()
        .beginClass<transform_system>("transform_system")
        .addStaticFunction("lerp", &transform_system::lerp)
        .addStaticFunction(
            "get_transform", +[](script_entity* ent) {
                transform_system::get_transform(entity(*ent->_scene, entity_t(ent->handle)));
            })
        .addStaticFunction(
            "move_to", +[](script_entity* ent, vec2 pos) {
                transform_system::move_to(entity(*ent->_scene, entity_t(ent->handle)), pos);
            })
        .endClass()
        .endNamespace();

    [[maybe_unused]] auto _ = push(L, &_input);
    lua_setglobal(L, "input");
    [[maybe_unused]] auto __ = push(L, &_eventQueue);
    lua_setglobal(L, "event_queue");
    register_math();
    register_types();

    logger::info("Scripting Engine created");
}

void scripting_engine::register_math()
{
    getGlobalNamespace(L)
        .beginNamespace("lemon")
        .beginClass<vec2>("vec2")
        .addConstructor<void(f32, f32)>()
        .addProperty("x", &vec2::x)
        .addProperty("y", &vec2::y)
        .endClass()
        .beginClass<ivec2>("ivec2")
        .addConstructor<void(i32, i32)>()
        .addProperty("x", &ivec2::x)
        .addProperty("y", &ivec2::y)
        .endClass()
        .beginClass<uvec2>("uvec2")
        .addConstructor<void(u32, u32)>()
        .addProperty("x", &uvec2::x)
        .addProperty("y", &uvec2::y)
        .endClass()
        .beginClass<vec3>("vec3")
        .addProperty("x", &vec3::x)
        .addProperty("y", &vec3::y)
        .addProperty("z", &vec3::z)
        .endClass()
        .beginClass<ivec3>("ivec3")
        .addProperty("x", &ivec3::x)
        .addProperty("y", &ivec3::y)
        .addProperty("z", &ivec3::z)
        .endClass()
        .beginClass<uvec3>("uvec3")
        .addProperty("x", &uvec3::x)
        .addProperty("y", &uvec3::y)
        .addProperty("z", &uvec3::z)
        .endClass()
        .beginClass<vec4>("vec4")
        .addProperty("x", &vec4::x)
        .addProperty("y", &vec4::y)
        .addProperty("z", &vec4::z)
        .addProperty("w", &vec4::w)
        .endClass()
        .beginClass<ivec4>("ivec4")
        .addProperty("x", &ivec4::x)
        .addProperty("y", &ivec4::y)
        .addProperty("z", &ivec4::z)
        .addProperty("w", &ivec4::w)
        .endClass()
        .beginClass<uvec4>("uvec4")
        .addProperty("x", &uvec4::x)
        .addProperty("y", &uvec4::y)
        .addProperty("z", &uvec4::z)
        .addProperty("w", &uvec4::w)
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

unordered_map<hashstr, function<i32(f32)>> scripting_engine::get_animation(const std::string& name)
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
