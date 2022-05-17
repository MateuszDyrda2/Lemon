#include <lemon/scripting/systems/scripting_system.h>

#include <entt/entt.hpp>
#include <lemon/core/game.h>
#include <lemon/core/instrumentor.h>
#include <lemon/scene/scene.h>
#include <lemon/scripting/py_script.h>

namespace lemon {
scripting_system::scripting_system(ptr<scene> s, clock& clk):
    clk(clk)
{
    s->get_registry()
        .on_construct<script_component>()
        .connect<entt::invoke<&script_component::instantiate>>();
}
scripting_system::~scripting_system()
{
}
void scripting_system::update(entity_registry& registry)
{
    LEMON_PROFILE_FUNCTION();
    float deltaTime = clk.delta_time();
    auto view       = registry.view<script_component>();
    (void)deltaTime;
    (void)view;
}
} // namespace lemon
