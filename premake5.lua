workspace "susumu"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "susumu/vendor/GLFW/include"

include "susumu/vendor/GLFW"

project "susumu"
	location "susumu"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "supch.h"
	pchsource "susumu/src/supch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS",
			"SU_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/sandbox")
		}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		optimize "On"
	
project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
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
	}

	links
	{
		"susumu"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		optimize "On"
	
