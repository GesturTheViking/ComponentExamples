workspace "Elia2D"
    configurations { "Debug", "Release" }
    platforms { "x64" }
    if (_ACTION == "vs2019") then
        location "VisualStudio"
    elseif (_ACTION == "gmake2") then
        location "Make"
    end

    startproject "AI2"
    cppdialect "C++20"

project "Engine"
    language "C++"
    targetdir "Bin/"
    targetname "Engine_%{cfg.buildcfg}"
    kind "StaticLib"
    links { "External/Library/glfw/glfw3", "OpenGL32", "GLu32" }

    files { "Source/Engine/**.h", "Source/Engine/**.cpp", "Source/Engine/**.hpp", "External/Source/**" }

    debugdir "Bin/"

    includedirs { "External/Headers", "External/Headers/ImGui" }

    toolset "clang"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines {"NDEBUG"}
        optimize "On"

project "ComponentSystem"
    language "C++"
    targetdir "Bin/"
    targetname "ComponentSystem_%{cfg.buildcfg}"
    kind "ConsoleApp"
    links { "Engine" }

    includedirs { "External/Headers", "External/Headers/ImGui", "Source/Engine" }

    files { "Source/ComponentSystem/**.h", "Source/ComponentSystem/**.cpp", "Source/ComponentSystem/**.hpp" }

    toolset "clang"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "EntityComponentSystem"
    language "C++"
    targetdir "Bin/"
    targetname "EntityComponentSystem_%{cfg.buildcfg}"
    kind "ConsoleApp"
    links { "Engine" }

    includedirs { "External/Headers", "External/Headers/ImGui", "Source/Engine" }

    files { "Source/EntityComponentSystem/**.h", "Source/EntityComponentSystem/**.cpp", "Source/EntityComponentSystem/**.hpp" }

    toolset "clang"

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"