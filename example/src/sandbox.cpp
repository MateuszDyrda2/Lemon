#include <lemon/entry_point.h>

#include <lemon/assets/asset_storage.h>
#include <lemon/events/event_handler.h>
#include <lemon/game/scene_manager.h>
#include <lemon/input/input.h>
#include <lemon/renderer/rendering_context.h>
#include <lemon/threads/scheduler.h>
#include <lemon/time/clock.h>
#include <lemon/window/window.h>

#include <lemon/serialization/scene_serializer.h>

#include <thread>

using namespace lemon;

class Sandbox : public engine
{
  public:
    Sandbox(int argc, char** argv);
    ~Sandbox();
    void initialize();
};

Sandbox::Sandbox(int argc, char** argv):
    engine(std::string(EXAMPLE_ROOT_DIR), argc, argv)
{
    _scheduler = create_owned<scheduler>(std::thread::hardware_concurrency() - 1);
    _events    = create_owned<event_handler>();
    _clock     = create_owned<lemon::clock>();
    _window    = create_owned<window>(1920, 1080);
    _input     = create_owned<input>(_window.get());
    rendering_context::create();
    _resources    = create_owned<asset_storage>();
    _sceneManager = create_owned<scene_manager>();
    this->initialize();
}
Sandbox::~Sandbox()
{
}
void Sandbox::initialize()
{
    engine::initialize();
    auto scene = _sceneManager->change_scene(string_id("SandboxScene"))
                     ->add_system<scripting_system>()
                     ->add_system<transform_system>()
                     ->add_system<rendering_system>();

    LOG_MESSAGE("Initialized %s", scene->get_name());

    auto gin = scene->add_entity(string_id("gin"));
    gin.add_component<sprite_renderer>(asset<texture>(string_id("gintoki")));

    scene_serializer::serialize(scene);
}
GAME_RUN(Sandbox);