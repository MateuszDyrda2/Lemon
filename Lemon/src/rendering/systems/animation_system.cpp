#include <rendering/systems/animation_system.h>

namespace lemon {
animation_system::animation_system(scene& _scene, event_queue& _eventQueue):
    _scene(_scene)
{
    update = _eventQueue["Render"_hs] += [this](event_args* e) {
        this->on_render(e);
    };
}

animation_system::~animation_system() { }

void animation_system::on_render([[maybe_unused]] event_args* e) noexcept
{
}

}
