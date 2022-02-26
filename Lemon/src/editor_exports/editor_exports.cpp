#include <lemon/editor_exports/editor_exports.h>

#include <lemon/game.h>
#include <lemon/serialization/scene_serializer.h>

lemon::editor_engine::editor_engine(const std::string& workingDirectory, int argc, char** argv):
    engine(workingDirectory, argc, argv)
{
    _events       = create_owned<event_handler>();
    _clock        = create_owned<lemon::clock>();
    _window       = create_owned<window_base>(1920, 1080);
    _resources    = create_owned<asset_storage>();
    _sceneManager = create_owned<scene_manager>();
    this->initialize();
}
lemon::editor_engine::~editor_engine() { }
void lemon::editor_engine::initialize()
{
    engine::initialize();
}
bool lemon::editor_engine::update()
{
    return true;
}
void lemon::editor_engine::drop() { }

using namespace lemon;
editor_engine* CreateEngine()
{
    return new editor_engine("Lemonade", 0, nullptr);
}
bool Update(editor_engine* engine)
{
    return engine->update();
}
void Initialize(editor_engine* engine)
{
    engine->initialize();
}
void Drop(editor_engine* engine)
{
    engine->drop();
}
void DeserializeScene(const char* name)
{
    game::get_scene_manager()
        ->change_scene(scene_serializer::deserialize(name))
        ->add_system<rendering_system>();
}
void SerializeScene()
{
}
