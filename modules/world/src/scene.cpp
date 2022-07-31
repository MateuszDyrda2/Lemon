#include <lemon/world/scene.h>

namespace lemon {
scene::scene(hash_str nameid):
    nameid(nameid)
{
}
scene::~scene()
{
}
void scene::update()
{
}
entity scene::create_entity(hash_str nameid)
{
    auto ent = world.create();
    return entity(ent, &world);
}
} // namespace lemon
