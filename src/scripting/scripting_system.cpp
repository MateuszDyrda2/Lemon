#include <river/scripting/scripting_system.h>

#include <entt/entt.hpp>
#include <river/game/basic_components.h>
#include <river/game/scene.h>

namespace river {
scripting_system::scripting_system(ptr<scene> s)
{
    s->get_registry()
        .on_construct<script_component>()
        .connect<entt::invoke<&script_component::on_create>>();
    s->get_registry()
        .on_destroy<script_component>()
        .connect<entt::invoke<&script_component::on_destroy>>();
}
scripting_system::~scripting_system()
{
}
void scripting_system::update(entity_registry& registry, float deltaTime)
{
    auto view = registry.view<script_component>();
    for(auto&& [ent, script] : view.each())
    {
        script.on_update(deltaTime);
    }
    for(auto&& [ent, script] : view.each())
    {
        script.on_late_update(deltaTime);
    }
}
} // namespace river
