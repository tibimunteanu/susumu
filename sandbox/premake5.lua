project "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/susumu/vendor/spdlog/include",
        "%{wks.location}/susumu/src",
        "%{wks.location}/susumu/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
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

