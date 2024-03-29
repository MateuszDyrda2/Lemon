#include "input_system.h"
#include "movement_system.h"
#include "physics/components/physics_components.h"
#include "player_components.h"
#include "enemy_system.h"
#include "rendering/components/rendering_components.h"
#include "world/components/entity_components.h"
#include "world/components/transform_components.h"
#include "world/systems/interpolate_system.h"
#include <core/hash_string.h>
#include <engine/engine.h>
#include <physics/systems/collision_system.h>
#include <physics/systems/physics_system.h>
#include <rendering/systems/animation_system.h>
#include <scripting/components/scripting_components.h>
#include <scripting/script.h>
#include <scripting/systems/scripting_system.h>

#include <rendering/systems/rendering_system.h>
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
    engine(argc, argv, ASSET_PATH, SETTINGS_FILE)
{
    initialize();
}

Sandbox::~Sandbox()
{ }

void Sandbox::initialize()
{
    logger::info("Sandbox created");
    auto& scene = _sceneManager.create_scene_defined("sandbox1", _serializer);
    scene.register_system<entity_system>()
        .register_system<physics_system>()
        .register_system<collision_system>()
        .register_system<interpolate_system>()
        .register_system<transform_system>()
        .register_system<animation_system>()
        .register_system<enemy_system>()
        .register_system<rendering_system>()
        .register_system<scripting_system>();

    scene.mount();
    // _sceneManager.create_scene("SandboxScene"_hs);

    // auto& scene = _sceneManager
    //                   .load_scene("SandboxScene"_hs)
    //                   .register_system<entity_system>()
    //                   //.register_system<movement_system>()
    //                   .register_system<physics_system>()
    //                   .register_system<collision_system>()
    //                   .register_system<interpolate_system>()
    //                   .register_system<transform_system>()
    //                   .register_system<animation_system>()
    //                   .register_system<rendering_system>()
    //                   .register_system<scripting_system>();

    // _serializer.register_all(scene.get_registry());
    // //_serializer.deserialize_scene(scene, SCENE_PATH "/sandbox1.json");

    // auto services = scene.get_services();

    // auto mainCamera = scene.create_entity(hashstr("Main Camera"));
    // mainCamera.emplace<main_camera_t>();
    // mainCamera.emplace<camera>();

    // auto player = scene.create_entity(hashstr("Player"));
    // player.emplace<player_t>();
    // auto&& psr      = player.emplace<sprite_renderer>();
    // psr.tex         = services._asset_storage.get_asset<texture>("player_anim"_hs);
    // const auto size = psr.tex.get()->get_size();
    // psr.texCoords   = { 0, 0, 48.f / size.x, 64.f / size.y };

    // auto&& rb       = player.emplace<rigidbody>();
    // rb.position     = player.get<transform>().position;
    // rb._oldPosition = rb.position;
    // rb.colliderType = collider_type::box;
    // rb.isKinetic    = false;
    // player.emplace<box_collider>(vec2{ 0.f, 0.f }, vec2{ 48.f, 64.f });

    // auto&& animation    = player.emplace<animation_component>();
    // animation.animation = services._asset_storage.get_asset<animation_script>("player_animation"_hs);
    // animation.frameSize = { 22, 56 };
    // player.emplace<start_animation_m>("idle"_hs);

    // auto&& scr       = player.emplace<script_component>();
    // scr.scriptObject = services._asset_storage.get_asset<script>("player_script"_hs);

    // _serializer.serialize_scene(scene, SCENE_PATH "/sandbox3.json");

    // scene.mount();
}

GAME_DECL(Sandbox);
