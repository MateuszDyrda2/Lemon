    externalproject "gtest"
    location "%{wks.location}/build/googletest/googletest"
    kind "StaticLib"
    language "C++"

group "Tests"
project "Unit"
    kind "ConsoleApp"
    cppdialect "C++20"
    files 
    {
        "main.cpp",
        "assets/assets_test.cpp",
        "core/hash_string_test.cpp",
        "core/logger_test.cpp",
        "events/events_test.cpp",
        "threading/scheduler_test.cpp",
    }

    targetdir ("%{wks.location}/build/%{prj.name}")
    objdir ("%{wks.location}/build/obj/%{prj.name}")

    links 
    { 
        "Lemon",
        "fmt",
        "glad",
        "gtest"
    }

    includedirs 
    {
        "%{wks.location}/Lemon/include",
        includes["concurrentqueue"],
        includes["fmt"],
        includes["glm"],
        includes["glad"],
        includes["googletest"],
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

