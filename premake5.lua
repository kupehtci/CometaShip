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


-- Include other Premake5 files for:
--          * GLAD initialization

include "vendor/glad"

project "CometaGL"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"

    includedirs {
        "%{IncludeDir.ImGUI}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.STB_IMAGE}",
        "vendor/imgui/backends/imgui_impl_glfw.h",
        "vendor/imgui/backends/imgui_impl_opengl3.h",
        "vendor/imgui/misc/cpp/imgui_stdlib.h",
        "src"
        --"vendor/GLFW/src"
--             "src/debug",
--             "src/core",
--             "src/components",
--             "src/math",
--             "src/physics",
--             "src/render",
--             "src/ui"
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


    targetdir ("bin/%{cfg.buildcfg}")
    objdir ("bin/obj/%{cfg.buildcfg}")

    -- MACOS specifications
    filter "system:macosx"

        includedirs{
            "/opt/homebrew/include",
            --"/vendor/GLFW/include/GLFW_macos"
        }

        libdirs{
            "vendor/glad/bin",
            "vendor/GLFW/lib_macos_arm"
        }

        links{
            "glfw3",
            "OpenGL.framework",
            "glad",
            "Cocoa.framework",
            "IOKit.framework"
        }

        defines { "PLATFORM_MACOS" }
    filter "system:linux"


    -- WINDOWS Expecifications
    filter "system:windows"
        --includedirs{
        --
        --}
        libdirs{
            "vendor/GLFW/lib"
        }
        links{
            "glfw3",
            "glad"
        }
        defines { "PLATFORM_WINDOWS" }


	filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

	filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
