izeurhif7zserizezieuezuruzeruzerzworkspace "Zap-Project"
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
    files { "../src/**.cpp"}
    files { "../src/**.h"}
    includedirs {"../extern/"}
    files {"../extern/stb_image.h"}
    includedirs {"../extern/DEPENDENCIES (glad)(GLFW)/include/"}
    cppdialect "C++17"


project "Application"
    location "Project/Application"
    kind "ConsoleApp"
    language "C++"
    files {"../application/main.cpp"}
    files {"../extern/glad.c"}
    includedirs {"../extern/DEPENDENCIES (glad)(GLFW)/include/"}
    includedirs {"../src/"}
    libdirs {"../extern/DEPENDENCIES (glad)(GLFW)/lib/"}
    libdirs {"../Project/Zap/bin/"}
    links {"Zap"}

    cppdialect "C++17"
    
    filter "system:windows"
        links {"glfw3","opengl32","user32", "gdi32","shell32"}
    
    filter "system:linux"
        links { "glfw", "GL", "dl", "pthread", "X11", "Xrandr", "Xi", "Xxf86vm", "m" }

    filter "system:macosx"
        links { "glfw", "glad" }
        linkoptions { "-framework OpenGL" }

   

