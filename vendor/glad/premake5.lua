
-- Include all glad library files as a premake 5 compilation
-- This library is then linked as an static library in the main Premake5 file

project "glad"
    kind "StaticLib"
    language "C"
    staticruntime "off"


    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin/obj/%{cfg.buildcfg}")


    filter "system:windows"
    systemversion "latest"

    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
    runtime "Release"
    optimize "on"