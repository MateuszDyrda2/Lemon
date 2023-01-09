
#include <engine/engine.h>
#include <world/systems/entity_system.h>
#include <world/systems/transform_system.h>
#include <world/systems/interpolate_system.h>
#include <physics/systems/collision_system.h>
#include <physics/systems/physics_system.h>
#include <rendering/systems/animation_system.h>
#include <rendering/systems/rendering_system.h>
#include <scripting/systems/scripting_system.h>

using namespace lemon;
class temp : public engine
{
  public:
    temp(int argc, char** argv);
    ~temp();
};

temp::temp(int argc, char** argv):
    engine(argc, argv, ASSET_PATH, SETTINGS_FILE)
{
    auto& _scene = _sceneManager.create_scene_defined("Scene1", _serializer);

    _scene.register_system<entity_system>()
        .register_system<physics_system>()
        .register_system<collision_system>()
        .register_system<interpolate_system>()
        .register_system<transform_system>()
        .register_system<animation_system>()
        .register_system<rendering_system>()
        .register_system<scripting_system>();

    _scene.mount();
}

temp::~temp() {}

GAME_DECL(temp);

