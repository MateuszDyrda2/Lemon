#include <river/entry_point.h>

#include <river/input/key_codes.h>

using namespace river;

RIVER_GAME_DECL(Sandbox)

RIVER_ON_CREATE(Sandbox)
{
    _sceneManager->push_scene(string_id("SandboxScene"));
    auto resourceManager = services::get<resource_manager>();
    auto tex             = resourceManager->load<texture>(string_id("Box"), "/home/matzix/proj/river/resources/textures/box.png");
    auto scn             = _sceneManager->get_current_scene();
    auto ent             = scn->add_entity(string_id("box"));
    ent.add_component<sprite_renderer>(tex);
}
RIVER_ON_DESTROY(Sandbox)
{
    services::get<resource_manager>()->unload(string_id("Box"));
}