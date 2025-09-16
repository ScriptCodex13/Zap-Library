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
    files { "../../src/**.cpp"}
    files { "../../src/**.h"}
    files { "../../src/Graphics/**.cpp"}
    files { "../../src/Graphics/**.h"}
    files { "../../src/GUI**.cpp"}
    files { "../../src/GUI/**.h"}
    files { "../../src/Util/**.cpp"}
    files { "../../src/Util/**.h"}
    files { "../../src/Window/**.cpp"}
    files { "../../src/Window/**.h"}
    includedirs {"../../extern/"}
    files {"../../extern/stb_image.h"}
    includedirs {"../../extern/glad/include/"}
    includedirs {"../../extern/GLFW/include/"}
    includedirs {"../../extern/freetype/include"}
    cppdialect "C++17"

    filter "configurations:Debug"
        staticruntime "off"
        runtime "Debug"


project "Application"
    location "Project/Application"
    kind "ConsoleApp"
    language "C++"
    files {"../../application/main.cpp"}
    files {"../../extern/glad.c"}
    includedirs {"../../extern/glad/include/"}
    includedirs {"../../extern/GLFW/include/"}
    includedirs {"../../extern/freetype/include"}
    includedirs {"../../src/"}
    includedirs {"../../extern/"}
    libdirs {"../../extern/GLFW/lib/Windows"}
    libdirs {"../../extern/freetype/lib/Windows"}
    libdirs {"../../Project/Zap/bin/"}
    links {"Zap"}

    cppdialect "C++17"

    filter "configurations:Debug"
        staticruntime "off"
        runtime "Debug"
    
    filter "system:windows"
        links {"glfw3","opengl32","user32", "gdi32","shell32", "freetype"} -- Add other platforms support with freetype later
    
    filter "system:linux"
        links { "glfw", "GL", "dl", "pthread", "X11", "Xrandr", "Xi", "Xxf86vm", "m" }

    filter "system:macosx"
        links { "glfw", "glad" }
        linkoptions { "-framework OpenGL" }

    
    
   





