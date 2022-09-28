#include <core/hash_string.h>
#include <engine/engine.h>

#include <rendering/systems/rendering_system.h>
#include <serialization/scene_serializer.h>
#include <world/systems/entity_system.h>
#include <world/systems/transform_system.h>

#include <components.h>

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

    auto& scene = _sceneManager.load_scene("SandboxScene"_hs)
                      .register_system<entity_system>(execution_stage::early_update)
                      .register_system<transform_system>(execution_stage::late_update)
                      .register_system<rendering_system>(execution_stage::render);

    auto& reg       = scene.get_registry();
    auto mainCamera = reg.create();
    reg.emplace<main_camera_t>(mainCamera);
    reg.emplace<camera>(mainCamera);
    reg.emplace<model>(mainCamera);
    reg.emplace<transform>(mainCamera);

    scene_serializer::serialize(
        &scene, "D:\\custom\\Lemon\\Sandbox\\scenes\\sandbox_scene.json", componentList);
}

GAME_DECL(Sandbox);