#include <lemon/engine/systems/player_system.h>

#include <lemon/scene/components/player_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
player_system::player_system(ptr<scene> s, input& i):
    i(i)
{
}
player_system::~player_system()
{
}
void player_system::update(entity_registry& registry)
{
    auto view = registry.view<player_controller>();
    view.each([this](auto ent, auto& controller) {
        for(auto&& [key, e] : controller.mapping)
        {
            if(i.is_key_pressed(key))
            {
                // TODO: Send event 'e'
            }
        }
    });
}
} // namespace lemon
