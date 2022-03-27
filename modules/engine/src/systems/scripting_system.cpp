#include <lemon/engine/systems/scripting_system.h>

#include <entt/entt.hpp>
#include <lemon/core/game.h>
#include <lemon/scene/scene.h>
#include <lemon/scripting/py_script.h>

namespace lemon {
scripting_system::scripting_system(ptr<scene> s)
{
    clk = game::get_game_clock();
    s->get_registry()
        .on_construct<script_component>()
        .connect<entt::invoke<&script_component::instantiate>>();
    /*
    s->get_registry()
        .on_destroy<script_component>()
        .connect<entt::invoke<&script_component::on_destroy>>(); */
}
scripting_system::~scripting_system()
{
}
void scripting_system::update(entity_registry& registry)
{
    float deltaTime = clk->delta_time();
    auto view       = registry.view<script_component>();
    for(auto&& [ent, script] : view.each())
    {
        script.update(deltaTime);
    }
    for(auto&& [ent, script] : view.each())
    {
        script.late_update(deltaTime);
    }
}
} // namespace lemon
