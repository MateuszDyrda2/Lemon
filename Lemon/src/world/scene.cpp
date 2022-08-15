#include <world/scene.h>

#include <algorithm>

namespace lemon {
scene::scene(hash_str nameid,
             asset_storage& _assetStorage, scheduler& _scheduler,
             event_queue& _eventQueue, window& _window):
    nameid(nameid),
    _assetStorage(_assetStorage),
    _scheduler(_scheduler),
    _eventQueue(_eventQueue),
    _window(_window)
{ }

scene::~scene()
{ }

void scene::on_load()
{ }

void scene::update()
{
    for(auto& sys : systems)
    {
        for(auto& s : sys)
        {
            s->update();
        }
    }
}

void scene::on_unload()
{ }
}