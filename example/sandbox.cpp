#include <lemon/entry_point.h>

#include <lemon/input/input.h>
#include <lemon/input/key_codes.h>
#include <lemon/job_system/job_system.h>
#include <lemon/window/window.h>

using namespace lemon;

class Sandbox : public engine
{
  public:
    Sandbox(int argc, char** argv);
    ~Sandbox();
    void initialize();
};

Sandbox::Sandbox(int argc, char** argv):
    engine(argc, argv)
{
    services::provide(create_owned<event_handler>());
    services::provide(create_owned<scheduler>(std::thread::hardware_concurrency()));
    services::provide(create_owned<resource_manager>());
    services::provide<window_base>(create_owned<window>("Sandbox", 1080, 720));
    services::provide(create_owned<input>());
    services::provide(create_owned<rendering_context>());
    services::provide(create_owned<scene_manager>());
    services::provide(create_owned<lemon::clock>());
    this->initialize();
}
Sandbox::~Sandbox()
{
    services::get<resource_manager>()->unload(string_id("Box"));
}
void Sandbox::initialize()
{
    engine::initialize();
    _sceneManager->push_scene(string_id("SandboxScene"));
    auto resourceManager = services::get<resource_manager>();
    auto tex             = resourceManager->load<texture>(string_id("Box"), "textures/box.png");
    auto scn             = _sceneManager->get_current_scene();
    auto ent             = scn->add_entity(string_id("box"));
    ent.add_component<sprite_renderer>(tex);
    LOG_MESSAGE("Initialized %s", _sceneManager->get_current_scene()->get_name());
}
GAME_RUN(Sandbox);