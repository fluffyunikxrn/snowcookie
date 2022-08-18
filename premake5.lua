-- premake5.lua

-- globals
snowcookie = "snowcookie"
corelib = "sce_corelib"
sandbox = "sce_sandbox"

-- workspace globals
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
bin = "bin/" .. outputdir
binint = "bin-int/" .. outputdir

-- workspace creation
workspace (snowcookie)
    architecture "x64"
    characterset ("unicode")
    configurations { "Debug", "Release", "Dist", "DebugDLL", "ReleaseDLL", "DistDLL" }
    cppdialect "C++20"
    -- defines {}
    language "C++"
    startproject(sandbox)

    filter "system:windows"
        systemversion "10.0.19041.0"
        defines { "_UNICODE", "UNICODE", "_CRT_SECURE_NO_WARNINGS" }

    filter "system:linux"

    filter "system:macosx"

-- prject snowcookie library
project (corelib)
    location "%{prj.name}"
    kind "SharedLib"
    language "C++"

    -- buildoptions "--std c++20"

    targetdir (bin .. "/%{prj.name}")
    objdir (binint .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.hxx", "%{prj.name}/src/**.c", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.cxx" }

    defines { "__SCE_EXPORT__" }

    pchheader "sce_pch.h"
    pchsource "src/sce_pch.cpp"

    includedirs { "%{prj.name}/src" }
    libdirs { "%{wks.location}/" .. bin .. corelib }

    postbuildcommands { "{COPY} %{cfg.buildtarget.directory}%{prj.name}.dll %{wks.location}" .. bin .. "/" .. sandbox }

    filter "configurations:Debug"
        defines { "DEBUG" }
    
    filter "configurations:Release"
        defines { "RELEASE", "NDEBUG" }

    filter "configurations:Release"
        defines { "DIST", "NDEBUG" }
    
    filter "system:windows"
        buildoptions  ("/Yc\"sce_pch.h\"")

-- project sandbox client
project (sandbox)
    dependson { corelib }
    location "%{prj.name}"
    kind "ConsoleApp"

    -- buildoptions "--std c++20"

    targetdir (bin .. "/%{prj.name}")
    objdir (binint .. "/%{prj.name}")

    files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.hxx", "%{prj.name}/src/**.c", "%{prj.name}/src/**.cpp", "%{prj.name}/src/**.cxx" }

    defines { "__SCE_IMPORT__" }

    includedirs { "%{prj.name}/src", ("%{wks.location}/" .. corelib .. "/src") }
    libdirs { "%{wks.location}/bin/" .. bin ..  corelib }

    links ( corelib )
   
    filter "configurations:Debug"
        defines { "DEBUG" }
    
    filter "configurations:Release"
        defines { "RELEASE", "NDEBUG" }

    filter "configurations:Release"
        defines { "DIST", "NDEBUG" }

