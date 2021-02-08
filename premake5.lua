include "./vendor/premake/premake_customization/solution_items.lua"

workspace "susumu"
    architecture "x86_64"
    startproject "susumu-editor"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items
    {
        ".editorconfig"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/susumu/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/susumu/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/susumu/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/susumu/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/susumu/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/susumu/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/susumu/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/susumu/vendor/ImGuizmo"
IncludeDir["assimp"] = "%{wks.location}/susumu/vendor/assimp/include"

group "Dependencies"
    include "vendor/premake"
    include "susumu/vendor/GLFW"
    include "susumu/vendor/Glad"
    include "susumu/vendor/imgui"
    include "susumu/vendor/yaml-cpp"
group ""

include "susumu"
include "sandbox"

