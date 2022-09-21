-- premake5.lua

-- globals
projName = "Snowcookie"
engineLib = "Engine"
sandboxExe = "Sandbox"

-- workspace globals
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
bin = "bin/" .. outputdir
binint = "bin-int/" .. outputdir

-- workspace creation
workspace (projName)
    architecture "x64"
    characterset ("unicode")
    configurations { "Debug", "Release", "Dist", "DebugDLL", "ReleaseDLL", "DistDLL" }
    cppdialect "C++20"
    language "C++"
    startproject(sandboxExe)

    filter "system:windows"
        systemversion "10.0.19041.0"
        defines { "_UNICODE", "UNICODE", "_CRT_SECURE_NO_WARNINGS" }

    filter "system:linux"

    filter "system:macosx"

-- prject snowcookie library
project (engineLib)
    location "%{prj.name}"
    kind "SharedLib"
    language "C++"

    -- buildoptions "--std c++20"

    targetdir (bin .. "/%{prj.name}")
    objdir (binint .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.hxx", "%{prj.name}/src/**.c", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.cxx" }

    defines { "SCE_EXPORT" }

    pchheader "sce_pch.h"
    pchsource "src/sce_pch.cpp"

    includedirs { "%{prj.name}/src" }
    libdirs { "%{wks.location}/" .. bin .. engineLib }

    postbuildcommands { "{COPY} %{cfg.buildtarget.directory}%{prj.name}.dll %{wks.location}" .. bin .. "/" .. sandboxExe }

    filter "configurations:Debug"
        defines { "DEBUG" }
    
    filter "configurations:Release"
        defines { "RELEASE", "NDEBUG" }

    filter "configurations:Release"
        defines { "DIST", "NDEBUG" }
    
    filter "system:windows"
        buildoptions  ("/Yc\"sce_pch.h\"")

    -- filter "system:linux"

    -- filter "system:macosx"

-- project sandbox client
project (sandboxExe)
    dependson { engineLib }
    location "%{prj.name}"
    kind "ConsoleApp"

    -- buildoptions "--std c++20"

    targetdir (bin .. "/%{prj.name}")
    objdir (binint .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.hxx", "%{prj.name}/src/**.c", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.cxx" }

    defines { "SCE_IMPORT" }

    includedirs { "%{prj.name}/src", ("%{wks.location}/" .. engineLib .. "/src") }
    libdirs { "%{wks.location}/bin/" .. bin ..  engineLib }

    links ( engineLib )
   
    filter "configurations:Debug"
        defines { "DEBUG" }
    
    filter "configurations:Release"
        defines { "RELEASE", "NDEBUG" }

    filter "configurations:Release"
        defines { "DIST", "NDEBUG" }

    -- filter "system:windows"

    -- filter "system:linux"

    -- filter "system:macosx"

