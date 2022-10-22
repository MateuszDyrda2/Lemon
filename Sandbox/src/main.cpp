#include "input_system.h"
#include "movement_system.h"
#include "physics/components/physics_components.h"
#include "player_components.h"
#include "rendering/components/rendering_components.h"
#include "world/components/entity_components.h"
#include "world/components/transform_components.h"
#include "world/systems/interpolate_system.h"
#include <core/hash_string.h>
#include <engine/engine.h>
#include <physics/systems/collision_system.h>
#include <physics/systems/physics_system.h>

#include <rendering/systems/rendering_system.h>
#include <serialization/scene_serializer.h>
#include <world/systems/entity_system.h>
#include <world/systems/transform_system.h>

using namespace lemon;
class Sandbox : public engine
{
  public:
    Sandbox(int argc, char** argv);
    ~Sandbox();

  private:
    void initialize();
};

Sandbox::Sandbox(int argc, char** argv):
    engine(argc, argv, ASSET_PATH)
{
    initialize();
}

Sandbox::~Sandbox()
{ }

void Sandbox::initialize()
{
    logger::info("Sandbox created");
    _sceneManager.create_scene("SandboxScene"_hs);

    auto& scene = _sceneManager
                      .load_scene("SandboxScene"_hs)
                      //.register_system<input_system>()
                      .register_system<entity_system>()
                      .register_system<physics_system>()
                      //.register_system<collision_system>()
                      .register_system<interpolate_system>()
                      .register_system<transform_system>()
                      .register_system<rendering_system>()
                      .register_system<movement_system>();

    auto services = scene.get_services();

    auto mainCamera = scene.create_entity(ENT_NAME("Main Camera"));
    mainCamera.emplace<main_camera_t>();
    mainCamera.emplace<camera>();

    /*    auto tile = scene.create_entity(ENT_NAME("Tile"));*/
    /*auto&& sr = tile.emplace<sprite_renderer>();*/
    /*sr.tex    = services._asset_storage.get_asset<texture>("tile"_hs);*/

    auto player = scene.create_entity(ENT_NAME("player"));
    player.emplace<player_t>();
    player.emplace<rigidbody>();
    auto&& psr = player.emplace<sprite_renderer>();
    psr.tex    = services._asset_storage.get_asset<texture>("player"_hs);
    transform_system::move_by(player, { -600, 0 });
    transform_system::scale_by(player, { 0.25, 0.25 });
}

GAME_DECL(Sandbox);
