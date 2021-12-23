#include <river/game/scene.h>

#include <algorithm>
#include <river/core/assert.h>

namespace river {
scene::scene(std::string const& name):
    object(name)
{
}
scene::~scene()
{
}
void scene::initialize()
{
    std::for_each(entities.begin(), entities.end(),
                  [](std::unique_ptr<entity>& ent) {
                      ent->awake_components();
                  });
}
void scene::begin()
{
    std::for_each(entities.begin(), entities.end(),
                  [](std::unique_ptr<entity>& ent) {
                      ent->start();
                  });
}
void scene::update()
{
    std::for_each(toDestroy.begin(), toDestroy.end(),
                  [this](size_type& index) {
                      entities.erase(entities.begin() + index);
                  });
    std::for_each(entities.begin(), entities.end(),
                  [](std::unique_ptr<entity>& ent) {
                      ent->destroy_pending();
                  });
}
entity& scene::add_entity(const std::string& name)
{
    return *entities.emplace_back(new entity(name, this));
}
void scene::destroy_entity(entity* ent)
{
    auto res = std::find_if(entities.begin(), entities.end(),
                            [=](std::unique_ptr<entity>& e) {
                                return e->get_id() == ent->get_id();
                            });
    RIVER_ASSERT(res != entities.end());
    toDestroy.push_back(size_type(res - entities.begin()));
}
} // namespace river
