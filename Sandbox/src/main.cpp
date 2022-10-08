#include "rendering/components/rendering_components.h"
#include "world/components/entity_components.h"
#include "world/components/transform_components.h"
#include <core/hash_string.h>
#include <engine/engine.h>

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
                      .register_system<entity_system>(execution_stage::early_update)
                      .register_system<transform_system>(execution_stage::late_update)
                      .register_system<rendering_system>(execution_stage::render);

    auto services = scene.get_services();

    auto mainCamera = scene.create_entity(ENT_NAME("Main Camera"));
    mainCamera.emplace<main_camera_t>();
    mainCamera.emplace<camera>();

    auto tile = scene.create_entity(ENT_NAME("Tile"));
    auto&& sr = tile.emplace<sprite_renderer>();
    sr.tex    = services._asset_storage.get_asset<texture>("tile"_hs);
    transform_system::move_by(tile, { 2, 1 });
}

GAME_DECL(Sandbox);
