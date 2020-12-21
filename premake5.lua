workspace "susumu"
    architecture "x86_64"
    startproject "sandbox"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "susumu/vendor/GLFW/include"
IncludeDir["Glad"] = "susumu/vendor/GLAD/include"
IncludeDir["ImGui"] = "susumu/vendor/imgui"
IncludeDir["glm"] = "susumu/vendor/glm"
IncludeDir["stb_image"] = "susumu/vendor/stb_image"

group "Dependencies"
    include "susumu/vendor/GLFW"
    include "susumu/vendor/Glad"
    include "susumu/vendor/imgui"
group ""

project "susumu"
    location "susumu"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "supch.h"
    pchsource "susumu/src/supch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "SU_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "SU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SU_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SU_DIST"
        runtime "Release"
        optimize "on"

project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "susumu/vendor/spdlog/include",
        "susumu/src",
        "susumu/vendor",
        "%{IncludeDir.glm}",
    }

    links
    {
        "susumu"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "SU_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SU_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "SU_DIST"
        runtime "Release"
        optimize "on"
