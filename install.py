import errno
import os
import shutil
import sys


def copyDir(src, dst):
    try:
        shutil.copytree(src, dst, dirs_exist_ok=True)
    except OSError as exc:
        if exc.errno in (errno.ENOTDIR, errno.EINVAL):
            shutil.copy(src, dst)
        else:
            raise

def copyFile(src, dst):
    shutil.copy(src, dst)


def main(argv):
    path = argv[1]
    lemonPath = os.path.join(path, "Lemon")
    libDir = os.path.join(lemonPath, "lib")
    binDir = os.path.join(lemonPath, "bin")
    docDir = os.path.join(lemonPath, "doc")
    licenseDir = os.path.join(lemonPath, "license")
    includeDir = os.path.join(lemonPath, "include")
    toolsDir = os.path.join(lemonPath, "tools")
    dataDir = os.path.join(lemonPath, "data")

    # documentation
    print("[1/8] Copying documentation to /doc ...")
    copyDir("build/lemon_doxygen", docDir)
    # include
    print("[2/8] Copying project headers to /include ...")
    copyDir("Lemon/include", includeDir)
    # binDir
    print("[3/8] Copying executable files to /bin ...")
    copyDir("build/Sandbox", os.path.join(binDir, "Sandbox"))
    copyDir("lemonade/src-tauri/target/release", os.path.join(binDir, "lemonade"))
    copyDir("Lemon/cmake", os.path.join(binDir, "cmake"))
    # docDir
    print("[4/8] Copying license to /license ...")
    shutil.copy("LICENSE", licenseDir)
    # toolsDir
    print("[5/8] Copying tools to /tools ...")
    copyDir("tools", toolsDir)
    os.mkdir(os.path.join(toolsDir, "components"))
    copyFile("Lemon/include/world/components/entity_components.h", os.path.join(toolsDir, "components"))
    copyFile("Lemon/include/world/components/transform_components.h", os.path.join(toolsDir, "components"))
    copyFile("Lemon/include/rendering/components/rendering_components.h", os.path.join(toolsDir, "components"))
    copyFile("Lemon/include/scripting/components/scripting_components.h", os.path.join(toolsDir, "components"))
    copyFile("Lemon/include/physics/components/physics_components.h", os.path.join(toolsDir, "components"))
    # libDir
    print("[6/8] Copying .lib files to /lib ...")
    copyDir("build/Lemon", libDir)
    # dataDir
    print("[7/8] Copying images to /data ...")
    copyDir("data", dataDir)

    # glfw
    print("[8/8] Copying external library headers to /include ...")
    copyDir("Lemon/libs/glfw/include/GLFW", os.path.join(includeDir, "GLFW"))
    copyFile("Lemon/libs/glfw/LICENSE.md", os.path.join(includeDir, "GLFW/LICENSE.md"))
    # concurrent_queue
    copyFile("Lemon/libs/concurrentqueue/concurrentqueue.h", includeDir)
    os.mkdir(os.path.join(includeDir, "concurrentqueue"))
    copyFile("Lemon/libs/concurrentqueue/LICENSE.md", os.path.join(includeDir, "concurrentqueue/LICENSE.md"))
    # fmt 
    copyDir("Lemon/libs/fmt/include/fmt", os.path.join(includeDir, "fmt"))
    copyFile("Lemon/libs/fmt/LICENSE.rst", os.path.join(includeDir, "fmt/LICENSE.rst"))
    # glad
    copyDir("Lemon/libs/glad/include/glad", os.path.join(includeDir, "glad"))
    # glm
    copyDir("Lemon/libs/glm/glm", os.path.join(includeDir, "glm"))
    copyFile("Lemon/libs/glm/copying.txt", os.path.join(includeDir, "glm/copying.txt"))
    # rapidjon
    copyDir("Lemon/libs/rapidjson/include/rapidjson", os.path.join(includeDir, "rapidjson"))
    copyFile("Lemon/libs/rapidjson/license.txt", os.path.join(includeDir, "rapidjson/license.txt"))
    # stb
    copyDir("Lemon/libs/stb/include/stb", os.path.join(includeDir, "stb"))
    # entt
    copyDir("Lemon/libs/entt/src/entt", os.path.join(includeDir, "entt"))
    copyFile("Lemon/libs/entt/LICENSE", os.path.join(includeDir, "entt/LICENSE"))
    # lua
    copyDir("Lemon/libs/lua", includeDir)
    # luabridge3
    copyDir("Lemon/libs/luabridge3/Source/LuaBridge", os.path.join(includeDir, "LuaBridge"))
    copyFile("Lemon/libs/luabridge3/LICENSE.txt", os.path.join(includeDir, "LuaBridge/LICENSE.txt"))

    # copy Lemon
    copyDir("Lemon", os.path.join(lemonPath, "Lemon"))
    

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Not enough arguments")
    else:
        main(sys.argv)
