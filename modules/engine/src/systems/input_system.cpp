#include "lemon/platform/key_codes.h"
#include <lemon/engine/systems/input_system.h>

#include <lemon/core/math/math.h>
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
    registry.clear<move_m>();
    auto view = registry.view<player_input>();
    view.each([&, this](auto ent, auto& player) {
        vec2 moveBy{};
        for(auto&& [keycode, modifier] : player.moveForward)
        {
            if(i.is_key_pressed(keycode))
                moveBy.y = modifier;
        }
        for(auto&& [keycode, modifier] : player.moveSideways)
        {
            if(i.is_key_pressed(keycode))
                moveBy.x = modifier;
        }
        if(moveBy.x != 0.f || moveBy.y != 0.f)
        {
            moveBy = normalize(moveBy);
            registry.emplace<move_m>(ent, moveBy);
        }
    });
}
} // namespace lemon
