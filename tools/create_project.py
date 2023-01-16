import shutil
import sys
import os
import json

main_template = '''
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
class {0} : public engine
{{
  public:
    {0}(int argc, char** argv);
    ~{0}();
}};

{0}::{0}(int argc, char** argv):
    engine(argc, argv, ASSET_PATH, SETTINGS_FILE)
{{
    auto& _scene = _sceneManager.create_scene_defined(\"Scene1\", _serializer);

    _scene.register_system<entity_system>()
        .register_system<physics_system>()
        .register_system<collision_system>()
        .register_system<interpolate_system>()
        .register_system<transform_system>()
        .register_system<animation_system>()
        .register_system<rendering_system>()
        .register_system<scripting_system>();

    _scene.mount();
}}

{0}::~{0}() {{}}

GAME_DECL({0});

'''

cmake_template = '''
cmake_minimum_required(VERSION 3.22)

project({0} LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${{CMAKE_CURRENT_SOURCE_DIR}}/cmake)

set({0}_SRC
    ${{CMAKE_CURRENT_SOURCE_DIR}}/src/main.cpp
)

set(COMPONENTS_LIST )

include(Macros)
set(serialization_files ${{CMAKE_BINARY_DIR}}/_generated/types.cpp)

set(COMPONENTS_LIST_LEMON
    "{1}/world/components/entity_components.h"
    "{1}/world/components/transform_components.h"
    "{1}/rendering/components/rendering_components.h"
    "{1}/scripting/components/scripting_components.h"
    "{1}/physics/components/physics_components.h"
)

add_components("${{COMPONENTS_LIST_LEMON}}")
add_components("${{COMPONENTS_LIST}}")
add_executable({0} ${{{0}_SRC}} ${{serialization_files}})

target_compile_definitions(
    {0} PUBLIC
    "ASSET_PATH=\\"${{CMAKE_CURRENT_SOURCE_DIR}}/assets\\""
    "SCENE_PATH=\\"${{CMAKE_CURRENT_SOURCE_DIR}}/scenes\\""
    "SETTINGS_FILE=\\"${{CMAKE_CURRENT_SOURCE_DIR}}/{0}.lmnproj\\""
)

'''


def main(argv):
    project_name = argv[1]
    project_dir = argv[2]

    os.mkdir(project_dir)

    src_path = project_dir + "/src"
    os.mkdir(src_path)
    asset_path = project_dir + "/assets"
    os.mkdir(asset_path)
    os.mkdir(asset_path + "/animations")
    os.mkdir(asset_path + "/scripts")
    os.mkdir(asset_path + "/shaders")
    os.mkdir(asset_path + "/textures")
    shutil.copytree("tools/cmake", os.path.join(asset_path, "cmake"))

    with open(asset_path + "/assets.json", "w+") as f:
        json.dump({
            "textures": [],
            "sounds": [],
            "shaders": [],
            "scripts": [],
            "animations": [],
        }, f, indent=2)

    os.mkdir(project_dir + "/scenes")
    with open(project_dir + "/scenes/Scene1.json", "w+") as f:
        json.dump({"entities": {"count": 0, "ids": []},
                  "components": []}, f, indent=2)

    with open(src_path + "/main.cpp", "w+") as f:
        f.write(main_template.format(project_name))

    cmake_file = project_dir + "/CMakeLists.txt"
    with open(cmake_file, "w+") as f:
        f.write(cmake_template.format(project_name, os.path.abspath("../../include")))

    project_f = {
        "project_name": project_name,
        "assets_path": "assets",
        "scene_path": "scenes",
        "src_path": "src",
        "types_path": "../build/_generated/types.json",
        "exec_path": "../build/Sandbox/sandbox",
        "scenes": ["Scene1"]
    }
    with open(project_dir + "/" + project_name + ".lmnproj", "w+") as f:
        json.dump(project_f, f, indent=2)


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)
