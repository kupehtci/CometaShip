-- Include file that defines "clean" action for removing build files
include "clean.lua"

-- Define main workspace
workspace "Cometa"
	configurations { "Debug", "Release" }

    -- Compile for x64 format. For 32 bits set to x86
    architecture "x86_64"

-- Array of the include directories
IncludeDir = {}
IncludeDir["GLM"] = "vendor/glm/glm"
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["GLAD"] = "vendor/glad/include"
IncludeDir["STB_IMAGE"] = "vendor/stb_image"
IncludeDir["ImGUI"] = "vendor/imgui"
IncludeDir["Assimp"] = "vendor/assimp/include"


-- Include other Premake5 files for:
--          * GLAD initialization

include "vendor/glad"

project "CometaFramework"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"

    includedirs {
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.Assimp}",
        "vendor/imgui/backends/imgui_impl_glfw.h",
        "vendor/imgui/backends/imgui_impl_opengl3.h",
        "vendor/imgui/misc/cpp/imgui_stdlib.h",
        "src"
    }

	files { 
            "src/**.h",
            "src/**.cpp",
            "src/**.vert",
            "src/**.frag",
            "%{IncludeDir.ImGUI}/*.h",
            "%{IncludeDir.ImGUI}/*.cpp",
            "vendor/imgui/backends/imgui_impl_glfw.h",
            "vendor/imgui/backends/imgui_impl_opengl3.h",
            "vendor/imgui/misc/cpp/imgui_stdlib.h",
            "vendor/imgui/backends/imgui_impl_glfw.cpp",
            "vendor/imgui/backends/imgui_impl_opengl3.cpp",
            "vendor/imgui/misc/cpp/imgui_stdlib.cpp"
    }

    libdirs{
        "vendor/assimp/include/assimp",
        "vendor/assimp/bin/Debug"
    }


    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin/obj/%{cfg.buildcfg}")

    -- MACOS specifications
    filter "system:macosx"

        includedirs{
            "/opt/homebrew/include"
        }

        libdirs{
            "vendor/glad/bin",
            "vendor/GLFW/lib_macos_arm",
            "/opt/homebrew/lib"
        }

        links{
            "glfw3",
            "OpenGL.framework",
            "glad",
            "Cocoa.framework",
            "IOKit.framework",
            "assimp"
        }

        defines { "PLATFORM_MACOS" }
    filter "system:linux"
        libdirs{
            "/usr/local/lib"
        }
        links{
            "assimp"
        }

    -- WINDOWS Expecifications
    filter "system:windows"
        includedirs{
            "vendor/assimp/include/assimp",
            "vendor/assimp/bin/Debug"
        }
        libdirs{
            "vendor/GLFW/lib",
            "vendor/assimp/lib/Debug",
            "vendor/assimp/bin/Debug"
        }
        links{
            "glfw3",
            "glad",
            "assimp-vc143-mtd"
            -- "assimp"
        }

        -- postbuildcommands {
        --     "{COPYFILE} vendor/assimp/bin/Debug/assimp-vc143-mtd.dll bin/%{cfg.targetdir}"
        -- }

        buildoptions { "/NODEFAULTLIB:MSVCRT" }
        defines { "PLATFORM_WINDOWS" }

	filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

	filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
