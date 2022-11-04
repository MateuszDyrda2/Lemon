#include <scripting/script.h>

#include <scripting/message_bus.h>

#include <filesystem>

extern "C"
{
#include <lauxlib.h>
#include <luajit.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

namespace lemon {
namespace fs = std::filesystem;

script::script(hash_str nameid, const std::string& path, scripting_engine& _scriptingEngine):
    resource(nameid),
    path(path), engine(_scriptingEngine)
{
    this->name = fs::path(path).stem();
    _scriptingEngine.load_file(path);
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
        lua_pushnumber(L, payload->asInteger);
        break;
    case message_payload::Float:
        lua_pushnumber(L, payload->asFloat);
        break;
    case message_payload::Bool:
        lua_pushboolean(L, payload->asBool);
        break;
    case message_payload::Vector:
        lua_pushlightuserdata(L, &payload->asVector);
        break;
    default:
        logger::warn("Incorrect function argument!");
    }
}

void script::execute(const std::string& func, message_payload* payload)
{
    auto L = engine.get_state();
    lua_getglobal(L, name.c_str());
    lua_getfield(L, -1, func.c_str());

    if (!lua_isfunction(L, -1))
    {
        lua_pop(L, 1);
        return;
    }
    auto p            = payload;
    std::size_t count = 0;
    while (p != nullptr)
    {
        lua_pushargument(L, p);
        p = p->next;
        ++count;
    }
    if (lua_pcall(L, count, 0, 0) != 0)
    {
        logger::error("Error running function {}: {}", func, lua_tostring(L, -1));
        return;
    }
}
}
