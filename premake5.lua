-- Generate project files with: bin\premake5.exe vs2026  (see README.md)

workspace "mu"
    location "."
    configurations { "Debug", "Release" }
    platforms { "x64", "x86" }
    startproject "mu"

    filter "platforms:x64"
        architecture "x86_64"
    filter "platforms:x86"
        architecture "x86"
    filter {}

project "mu"
    location "src"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"
    characterset "Unicode"
    conformancemode "On"
    multiprocessorcompile "On"
    warnings "Default"

    filter "toolset:msc*"
        buildoptions { "/sdl" }
    filter {}

    targetdir "%{wks.location}/%{cfg.platform}/%{cfg.buildcfg}"
    objdir    "%{wks.location}/%{cfg.platform}/%{cfg.buildcfg}/intermediates"

    files { "src/**.h", "src/**.cpp" }

    defines { "_CONSOLE" }

    filter "platforms:x86"
        defines { "WIN32" }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        symbols "On"
        optimize "Speed"
        runtime "Release"
        linktimeoptimization "On"
        functionlevellinking "On"
        intrinsics "On"

    filter {}
