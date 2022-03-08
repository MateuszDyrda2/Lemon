#pragma once

#include <lemon/assets/resource.h>
#include <lemon/core/basic_types.h>
#include <lemon/scene/entity.h>

#include <functional>
#include <pybind11/embed.h>

namespace lemon {
namespace py = pybind11;
class LEMON_PUBLIC py_script
{
  public:
    py_script(const std::string& moduleName);
    void instantiate(entity ent);
    ~py_script() = default;

  private:
    friend class script_component;

    py::function onCreate;
    py::function onUpdate;
    py::function onLateUpdate;
    py::function onDestroy;

    entity ent;
    py::module_ mod;
};
struct LEMON_PUBLIC script_component
{
    py_script* script{};

    script_component(const std::string& moduleName):
        script(new py_script(moduleName)) { }

    script_component(const script_component&) = delete;
    script_component& operator=(const script_component&) = delete;
    script_component(script_component&& other) noexcept:
        script(other.script) { other.script = nullptr; }
    script_component& operator=(script_component&& other) noexcept
    {
        if(this != &other)
        {
            script       = other.script;
            other.script = nullptr;
        }
        return *this;
    }

    friend void swap(script_component& lhs, script_component& rhs)
    {
        std::swap(lhs.script, rhs.script);
    }

    inline void instantiate(entity_registry& reg, entity_handle h)
    {
        script->instantiate(entity(&reg, h));
        script->onCreate();
    }
    inline void update(f32 delta)
    {
        script->onUpdate(delta);
    }
    inline void late_update(f32 delta)
    {
        script->onLateUpdate(delta);
    }
    ~script_component()
    {
        if(script)
        {
            script->onDestroy();
            delete script;
        }
    }
};
} // namespace lemon
