workspace "Zap-Project"
    architecture "x64"

    location "ProjectAnder"

    configurations
    {
        "Debug",
        "Release"
    }

project "Zap"
    location "ProjectAnder/Zap"
    kind "StaticLib"
    language "C++"
    files { "../src/**.cpp"}
    files { "../src/**.h"}
    includedirs {"../3rdparty/glad.gl4.6/include/"}
    includedirs {"../3rdparty/glfw-3.4.bin.WIN64/include"}
    includedirs {"../3rdparty/glm-0.9.9.7/glm/"}
    includedirs {"../3rdparty/stb"}
    files {"../extern/stb_image.h"}
    cppdialect "C++17"


project "Application"
    location "ProjectAnder/Application"
    kind "ConsoleApp"
    language "C++"
    files {"../application/main.cpp"}
    files {"../3rdparty/glad.gl4.6/src/glad.c"}
    includedirs {"../3rdparty/glad.gl4.6/include/"}
    includedirs {"../3rdparty/glfw-3.4.bin.WIN64/include"}
    includedirs {"../3rdparty/glm-0.9.9.7/glm/"}
    includedirs {"../3rdparty/stb"}
    includedirs {"../src/"}
    libdirs {"../3rdparty/glfw-3.4.bin.WIN64/lib-vc2022/"}
    libdirs {"../ProjectAnder/Zap/bin/"}
    links {"Zap"}

    cppdialect "C++17"
    
    filter "system:windows"
        links {"glfw3_mt","opengl32","user32", "gdi32","shell32"}
    
    filter "system:linux"
        links { "glfw", "GL", "dl", "pthread", "X11", "Xrandr", "Xi", "Xxf86vm", "m" }

    filter "system:macosx"
        links { "glfw", "glad" }
        linkoptions { "-framework OpenGL" }

   



