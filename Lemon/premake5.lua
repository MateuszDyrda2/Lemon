include "libs/libraries.lua"

project "Lemon"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    dependson { "fmt", "glm", "glfw" }
    
    targetdir ("%{wks.location}/build/%{prj.name}")
    objdir ("%{wks.location}/build/obj/%{prj.name}")

    os.execute("cmake -S " .. _WORKING_DIR .. "/Lemon/libs -B" .. _WORKING_DIR .. "/build")

    postbuildcommands
    {
        {"{COPY} %{cfg.buildtarget.relpath} %{wks.location}/build/Sandbox"},
        {"{COPY} %{cfg.buildtarget.relpath} %{wks.location}/build/Unit"},
    }

    files
    {
        "src/**.cpp",
        "include/**.h"
    }

    includedirs
    {
        "include/",
        includes["concurrentqueue"],
        includes["fmt"],
        includes["glad"],
        includes["glm"],
        includes["glfw"],
        includes["rapidjson"],
        includes["stb"],
        includes["entt"],
    }

    links
    {
        "fmt",
        "glad",
        "glfw",
    }

    if os.ishost("windows") then
        defines 
        {
            "LEMON_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
            "LEMON_EXPORTS"
        }
        buildoptions { "/wd4251" }

    elseif os.ishost("linux") then
        defines { "LEMON_LINUX" }
    end

    filter "configurations:Debug"
        defines "LEMON_DEBUG"
        runtime "Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        defines "LEMON_RELEASE"
        runtime "Release"
        optimize "On"
        symbols "Off"
