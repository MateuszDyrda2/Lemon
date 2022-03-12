#include <lemon/engine/entry_point.h>

#include <lemon/core/game.h>
#include <lemon/platform/window.h>

#include <lemon/engine/systems/audio_system.h>
#include <lemon/engine/systems/physics_system.h>
#include <lemon/engine/systems/rendering_system.h>
#include <lemon/engine/systems/scripting_system.h>
#include <lemon/engine/systems/transform_system.h>

#include <lemon/serialization/scene_serializer.h>

#include <thread>

#include <lemon/platform/key_codes.h>
#include <lemon/platform/window_events.h>
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
    _events           = create_owned<event_handler>();
    _clock            = create_owned<lemon::clock>();
    _window           = create_owned<window>(1920, 1080);
    _input            = create_owned<input>(_window.get());
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

    auto scene = _sceneManager->change_scene(string_id("SandboxScene"))
                     ->add_system<scripting_system>()
                     ->add_system<physics_system>()
                     ->add_system<transform_system>()
                     ->add_system<audio_system>()
                     ->add_system<rendering_system>();

    LOG_MESSAGE("Initialized %s", scene->get_id().get_string());

    auto gin = scene->add_entity(string_id("gin"));
    gin.add_component<sprite_renderer>(asset<texture>(string_id("gin2")));
    gin.add_component<script_component>("gin");
    gin.add_component<box_collider>();
    gin.add_component<rigidbody>();

    // gin.add_component<audio_source>(asset<sound>(string_id("open-the-door")));
    // audio_system::begin_play(gin);
    //  scene_serializer::serialize(scene);
}
GAME_RUN(Sandbox);