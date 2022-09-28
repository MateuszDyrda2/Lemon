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
        "generated/",
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

    filter 'files:**components.h'
        -- message
        buildmessage 'Generating serialization data for %{file.relpath}'

        buildcommands {
        'py.exe %{wks.location}/tools/export_components.py "_generated/types.json" "%{file.relpath}"'
        }

        buildoutputs {
            '_generated/types.json'
        }

    filter 'files:**system.h'
        -- message
        buildmessage 'Generating serialization data for %{file.relpath}'

        buildcommands {
        'py.exe %{wks.location}/tools/export_systems.py "_generated/types.json" "%{file.relpath}"'
        }

        buildoutputs {
            '_generated/types.json'
        }

    filter 'files:_generated/types.json'
        buildmessage 'Generating _generated/components.h'

        buildcommands {
        'py.exe %{wks.location}/tools/write_component_file.py "%{file.relpath}" "_generated/components.h"'
        }
        buildoutputs {
            '_generated/components.h'
        }

