#include "lemon/platform/key_codes.h"
#include <lemon/engine/systems/input_system.h>

#include <lemon/scene/components/player_components.h>
#include <lemon/scene/scene.h>

namespace lemon {
input_system::input_system(ptr<scene> /*s*/, input& i):
    i(i)
{
}
input_system::~input_system()
{
}
void input_system::update(entity_registry& registry)
{
    auto view = registry.view<player_input>();
    view.each([&, this](auto ent, auto& player) {
        vec2 moveBy{};
        if(i.is_key_pressed(player.moveUp))
        {
            moveBy.y += 1.f;
        }
        if(i.is_key_pressed(player.moveDown))
        {
            moveBy.y -= 1.f;
        }
        if(i.is_key_pressed(player.moveLeft))
        {
            moveBy.x -= 1.f;
        }
        if(i.is_key_pressed(player.moveRight))
        {
            moveBy.x += 1.f;
        }
        registry.emplace<move_by_m>(ent, moveBy);
    });
}
} // namespace lemon
