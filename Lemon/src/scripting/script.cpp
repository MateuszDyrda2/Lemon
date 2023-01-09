#include <scripting/script.h>

#include <scripting/message_bus.h>

#include <filesystem>

extern "C"
{
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

namespace lemon {
namespace fs = std::filesystem;

script::script(hashstr nameid, const std::string& path, scripting_engine& _scriptingEngine):
    resource(nameid),
    path(path), engine(_scriptingEngine)
{
    this->name = fs::path(path).stem().string();
    _scriptingEngine.load_file(path);

    auto L            = engine.get_state();
    auto entNamespace = luabridge::getGlobal(L, name.c_str());
    auto onUpdate     = entNamespace.rawget("on_update");
    if (onUpdate.isCallable())
    {
        update_f = [onUpdate = std::move(onUpdate)](script_entity ent, f32 delta) {
            onUpdate(ent, delta);
        };
    }

    auto onEarly = entNamespace.rawget("on_early_update");
    if (onEarly.isCallable())
    {
        early_f = [onEarly = std::move(onEarly)](script_entity ent, f32 delta) {
            onEarly(ent, delta);
        };
    }

    auto onPhysics = entNamespace.rawget("on_physics");
    if (onPhysics.isCallable())
    {
        physics_f = [onPhysics = std::move(onPhysics)](script_entity ent, f32 delta) {
            onPhysics(ent, delta);
        };
    }
}

script::~script()
{
}

script::script(script&& other) noexcept:
    resource(std::move(other)), path(std::move(other.path)),
    name(std::move(other.name)), engine(other.engine)
{ }

script& script::operator=(script&& other) noexcept
{
    if (this != &other)
    {
        resource::operator=(std::move(other));
        path = std::move(other.path);
        name = std::move(other.name);
    }
    return *this;
}

void lua_pushargument(lua_State* L, message_payload* payload)
{
    switch (payload->payloadType)
    {
    case message_payload::Integer:
    case message_payload::HashString:
    {
        auto res = luabridge::push(L, payload->asInteger);
        if (!res)
        {
            logger::error("Error passing argument {} to lua", payload->asInteger);
        }
    }
    break;
    case message_payload::Float:
    {
        auto res = luabridge::push(L, payload->asFloat);
        if (!res)
        {
            logger::error("Error passing argument {} to lua", payload->asFloat);
        }
    }
    break;
    case message_payload::Bool:
    {

        auto res = luabridge::push(L, payload->asBool);
        if (!res)
        {
            logger::error("Error passing argument {} to lua", payload->asBool);
        }
    }
    break;
    case message_payload::Vector:
    {
        auto res = luabridge::push(L, payload->asVector);
        if (!res)
        {
            logger::error("Error passing argument {}x{} to lua", payload->asVector.x, payload->asVector.y);
        }
    }
    break;
    default:
        logger::warn("Incorrect function argument!");
    }
}

void script::execute(script_entity ent, const std::string& func, message_payload* payload)
{
    auto L = engine.get_state();

    lua_getglobal(L, name.c_str());
    lua_getfield(L, -1, func.c_str());

    if (!lua_isfunction(L, -1))
    {
        // lua_pop(L, 2);
        lua_remove(L, -2);
        return;
    }
    auto p            = payload;
    std::size_t count = 1;
    auto res          = luabridge::push(L, ent);
    if (!res)
    {
        logger::error("Error passing argument {} to {} in {}", count, func, name);
        return;
    }
    while (p != nullptr)
    {
        lua_pushargument(L, p);
        p = p->next;
        ++count;
        luaL_checkstack(L, 1, "too many args");
    }
    if (lua_pcall(L, int(count), 0, 0) != 0)
    {
        logger::error("Error running function {}: {}", func, lua_tostring(L, -1));
        return;
    }
}

void script::update(script_entity ent, f32 delta)
{
    if (!update_f) return;
    update_f(ent, delta);
}

void script::early(script_entity ent, f32 delta)
{
    if (!early_f) return;
    early_f(ent, delta);
}

void script::physics(script_entity ent, f32 delta)
{
    if (!physics_f) return;
    physics_f(ent, delta);
}

void script::start(script_entity ent)
{
    auto L            = engine.get_state();
    auto entNamespace = luabridge::getGlobal(L, name.c_str());
    auto start        = entNamespace.rawget("on_start");
    if (!start.isCallable()) return;

    auto res = start(ent);
    if (res.hasFailed())
    {
        logger::warn("Lua script {} failed with {}", name, res.errorMessage());
    }
}
}
