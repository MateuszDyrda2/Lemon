libs_location   = "%{wks.location}/Lemon/libs/"
includes        = {}

includes["fmt"]             = libs_location .. "fmt/include"
includes["concurrentqueue"] = libs_location .. "concurrentqueue"
includes["glad"]            = libs_location .. "glad/include"
includes["glm"]             = libs_location .. "glm"
includes["glfw"]            = libs_location .. "glfw/include"
includes["rapidjson"]       = libs_location .. "rapidjson/include"
includes["stb"]             = libs_location .. "stb/include"
includes["googletest"]      = libs_location .. "googletest/googletest/include"
includes["entt"]            = libs_location .. "entt/src"

externalproject "fmt"
    location "%{wks.location}/build/fmt"
    kind "StaticLib"
    language "C++"

externalproject "glfw"
    location "%{wks.location}/build/glfw/src"
    kind "StaticLib"
    language "C++"

externalproject "glad"
    location "%{wks.location}/build/glad"
    kind "StaticLib"
    language "C++"
