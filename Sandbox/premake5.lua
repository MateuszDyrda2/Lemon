project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    targetdir ("%{wks.location}/build/%{prj.name}")
    objdir ("%{wks.location}/build/obj/%{prj.name}")

    files 
    { 
        "src/**.h",
        "src/**.cpp",
        "./libs/fmt/include"
    }

    includedirs 
    {
        "%{wks.location}/Lemon/include",
        includes["concurrentqueue"],
        includes["fmt"],
        includes["glm"],
        includes["glad"],
        includes["entt"],
        includes["rapidjson"],
    }

    links 
    { 
        "Lemon",
        "fmt",
        "glad",
    }

    defines
    {
        "ASSET_PATH=\"" .. _WORKING_DIR .. "/%{prj.name}/assets\""
    }

    if os.ishost("windows") then
        defines 
        {
            "LEMON_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS",
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
