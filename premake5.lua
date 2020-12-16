workspace "susumu"
	architecture "x64"
	startproject "sandbox"

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
IncludeDir["Glad"] = "susumu/vendor/GLAD/include"
IncludeDir["ImGui"] = "susumu/vendor/imgui"

group "Dependencies"
	include "susumu/vendor/GLFW"
	include "susumu/vendor/Glad"
	include "susumu/vendor/imgui"
group ""

project "susumu"
	location "susumu"
	kind "SharedLib"
	language "C++"
	staticruntime "Off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS",
			"SU_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/sandbox/\"")
		}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "On"
	
project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

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
		systemversion "latest"

		defines
		{
			"SU_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SU_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SU_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SU_DIST"
		runtime "Release"
		optimize "On"
	
