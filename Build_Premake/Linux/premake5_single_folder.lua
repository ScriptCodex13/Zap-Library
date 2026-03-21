workspace "Zap-Project"
    architecture "x64"

    location "Project"

    configurations
    {
        "Debug",
        "Release"
    }

project "Zap"
    location "Project/Zap"
    kind "StaticLib"
    language "C++"
    files { "../../zap_lib/**.cpp"}
    files { "../../zap_lib/**.h"}
    files { "../../zap_lib/Graphics/**.cpp"}
    files { "../../zap_lib/Graphics/**.h"}
    files { "../../zap_lib/GUI**.cpp"}
    files { "../../zap_lib/GUI/**.h"}
    files { "../../zap_lib/Util/**.cpp"}
    files { "../../zap_lib/Util/**.h"}
    files { "../../zap_lib/Window/**.cpp"}
    files { "../../zap_lib/Window/**.h"}
    files { "../../zap_lib/Audio/**.cpp"}
    files { "../../zap_lib/Audio/**.h"}
    includedirs {"../../extern/miniaudio/"}
    includedirs {"../../extern/glad/include/"}
    includedirs {"../../extern/GLFW/include/"}
    includedirs {"../../extern/freetype/include/Windows"}
    includedirs {"../../extern/"}
    includedirs {"../../extern/assimp/include"}
    cppdialect "C++17"

    filter "action:vs*"
        flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        staticruntime "off"
        runtime "Debug"


project "Application"
    location "Project/Application"
    kind "ConsoleApp"
    language "C++"
    files {"../../application/enabler.h"}
    files {"../../application/main.cpp"}
    files {"../../application/main_*.cpp"}
    files {"../../application/Cube.*"}
    files {"../../application/LightCube.*"}
    files {"../../extern/glad.c"}
    includedirs {"../../extern/glad/include/"}
    includedirs {"../../extern/GLFW/include/"}
    includedirs {"../../extern/freetype/include/Windows"}
    includedirs {"../../zap_lib/"}
    includedirs {"../../extern/"}
    includedirs {"../../extern/miniaudio/"}
    includedirs {"../../extern/assimp/include"}
    links {"Zap"}

    cppdialect "C++17"

    filter "action:vs*"
        flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        links {"zlibstaticd"}
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        links {"zlibstatic"}
        runtime "Release"
    
    
    filter "system:linux"
        links {
            "glfw",
            "GL",
            "dl",
            "pthread",
            "X11",
            "Xrandr",
            "Xi",
            "Xxf86vm",
            "Xcursor",
            "Xinerama",
            "freetype",
            "assimp",
            "m"
        }
    
   





