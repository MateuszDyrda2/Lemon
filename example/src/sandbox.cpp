#include <lemon/engine/entry_point.h>

#include <lemon/core/game.h>
#include <lemon/events/event_bus.h>
#include <lemon/platform/window.h>
#include <lemon/threading/scheduler.h>

#include <lemon/audio/systems/audio_system.h>
#include <lemon/engine/systems/debug_system.h>
#include <lemon/engine/systems/input_system.h>
#include <lemon/physics/systems/collision_response_system.h>
#include <lemon/physics/systems/collision_system.h>
#include <lemon/physics/systems/movement_system.h>
#include <lemon/physics/systems/physics_system.h>
#include <lemon/rendering/systems/rendering_system.h>
#include <lemon/scene/systems/entity_system.h>
#include <lemon/scene/systems/transform_system.h>
#include <lemon/scripting/systems/scripting_system.h>

#include <lemon/audio/components/audio_components.h>
#include <lemon/physics/components/physics_components.h>
#include <lemon/rendering/components/rendering_components.h>
#include <lemon/scene/components/entity_components.h>
#include <lemon/scene/components/player_components.h>
#include <lemon/scene/components/transform_components.h>
#include <lemon/scripting/components/scripting_components.h>

#include <lemon/serialization/scene_serializer.h>

#include <lemon/platform/key_codes.h>
#include <lemon/scripting/py_script.h>

using namespace lemon;

class Sandbox : public engine
{
  public:
    Sandbox(int argc, char** argv);
    ~Sandbox();
    void initialize();
};

Sandbox::Sandbox(int argc, char** argv):
    engine(std::string(EXAMPLE_ROOT_DIR) + "/lemon.json", argc, argv)
{
    _scriptingEngine  = create_owned<py_engine>();
    _scheduler        = create_owned<scheduler>(std::thread::hardware_concurrency() - 1);
    _eventBus         = create_owned<event_bus>();
    _clock            = create_owned<lemon::clock>();
    _window           = create_owned<window>(1920, 1080, *_eventBus, *_scheduler);
    _input            = create_owned<input>(*_window);
    _renderingContext = create_owned<rendering_context>();
    _soundContext     = create_owned<sound_context>();
    _resources        = create_owned<asset_storage>();
    _sceneManager     = create_owned<scene_manager>();

    game::provide_clock(_clock.get());
    game::provide_window(_window.get());
    game::provide_scene_manager(_sceneManager.get());
    this->initialize();
}
Sandbox::~Sandbox()
{
}
void Sandbox::initialize()
{
    engine::initialize();
    /*
        auto scene = _sceneManager->change_scene(scene_serializer::deserialize(game::get_settings().startingScene))
                         ->add_system<scripting_system>()
                         ->add_system<transform_system>()
                         ->add_system<audio_system>()
                         ->add_system<rendering_system>();

        LOG_MESSAGE("Initialized %s", scene->get_id().get_string());

        scene->get_registry().emplace<script_component>(entt::entity(0), "gin");*/
    // test.add_component<script_component>("gin");

    /*    auto scene = _sceneManager->change_scene(string_id("SandboxScene"))
                         ->add_system<scripting_system>(*_clock)
                         ->add_system<physics_system>(*_clock, *_scheduler)
                         ->add_system<transform_system>()
                         ->add_system<audio_system>()
                         ->add_system<rendering_system>(*_eventBus)
                         ->add_system<debug_system>(*_eventBus);
                         */
    auto scene = _sceneManager->change_scene(string_id("SandboxScene"))
                     ->add_system<entity_system>()
                     ->add_system<input_system>(*_input)
                     ->add_system<movement_system>(*_clock, *_scheduler)
                     ->add_system<collision_system>(*_scheduler)
                     ->add_system<collision_response_system>(*_clock, *_scheduler)
                     ->add_system<physics_system>(*_clock, *_scheduler)
                     ->add_system<transform_system>()
                     ->add_system<rendering_system>(*_eventBus)
                     ->add_system<debug_system>(*_eventBus);

    LOG_MESSAGE("Initialized %s", scene->get_id().get_string());
    auto mainCamera = scene->add_entity(string_id("mainCamera"));
    mainCamera.add_component<camera>();
    mainCamera.add_component<main_camera_t>();
    mainCamera.add_component<audio_listener>();

    auto gin = scene->add_entity(string_id("gin"));
    gin.add_component<sprite_renderer>(asset<texture>(string_id("gin2")));
    auto& pi = gin.add_component<player_input>();
    pi.moveForward.push_back({ key::keycode::w, 1.f });
    pi.moveForward.push_back({ key::keycode::s, -1.f });
    pi.moveSideways.push_back({ key::keycode::d, 1.f });
    pi.moveSideways.push_back({ key::keycode::a, -1.f });

    auto& ec          = gin.add_component<entity_controller>();
    ec.speed          = 150.f;
    auto& coll        = gin.add_component<collider>(vec2(0.f, -.4f), vec2(.1f, .1f));
    auto& rb          = gin.add_component<rigidbody>();
    rb.freezeRotation = true;
    rb.linearDrag     = 20.f;

    // auto plane = scene->add_entity(string_id("plane"));
    auto plane = scene->add_entity(string_id("plane"), { 0.f, -2.f }, { 1.f, 1.f }, 0.f);
    plane.add_component<sprite_renderer>(asset<texture>(string_id("plane")));
    plane.add_component<collider>(vec2(0.f, 0.f), vec2(5.6f, .4f));
    // plane.patch_component<transform>([](transform& tr) { tr.position = { 0.f, -200.f }; });

    // gin.add_component<audio_source>(asset<sound>(string_id("open-the-door")));
    // audio_system::begin_play(gin);
    // scene_serializer::serialize(scene);
}
GAME_RUN(Sandbox);