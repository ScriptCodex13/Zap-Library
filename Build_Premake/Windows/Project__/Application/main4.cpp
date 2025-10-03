//#define ZAP_LIBRARY_MAIN4_CPP
#ifdef ZAP_LIBRARY_MAIN4_CPP
#include <atlbase.h>
#include <atlwin.h>
#include <iostream>
#include <Zap.h>
#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

const char* vertexShaderXText = R"glsl(#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
})glsl";
const char* fragmentShaderXText = R"glsl(#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r); //r from rgba
    color = vec4(textColor, 1.0) * sampled; //vec4(vec3(0.5, 0.8f, 0.2f).rgb, 1.0) * vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
})glsl";



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

///// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

unsigned int buildSubTexture(FT_Bitmap& btm) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        btm.width,
        btm.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        btm.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}
void buildTextInfo(FT_Face face)
{
    assert(!FT_Set_Pixel_Sizes(face, 0, 48));

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        assert(!FT_Load_Char(face, c, FT_LOAD_RENDER));
        // generate texture
        // now store character for later use
        FT_GlyphSlot& glyph = face->glyph;
		FT_Bitmap& btm = face->glyph->bitmap;
        Characters[c] = {
            buildSubTexture(btm),
            glm::ivec2(btm.width, btm.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            static_cast<unsigned int>(glyph->advance.x)
        };
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}
static void buildTextInfo()
{
    FT_Library ft;
    FT_Face face;
    assert(!FT_Init_FreeType(&ft));
    assert(!FT_New_Face(ft, "C:/Windows/Fonts/arial.ttf", 0, &face));
    zap::util::scope_guard sg_freetype([&face, &ft]() { FT_Done_Face(face); FT_Done_FreeType(ft); });

    assert(!FT_Set_Pixel_Sizes(face, 0, 48));

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        assert(!FT_Load_Char(face, c, FT_LOAD_RENDER));
        // generate texture
        // now store character for later use
        FT_GlyphSlot& glyph = face->glyph;
		FT_Bitmap& btm = face->glyph->bitmap;
        Characters[c] = {
            buildSubTexture(btm),
            glm::ivec2(btm.width, btm.rows),
            glm::ivec2(glyph->bitmap_left, glyph->bitmap_top),
            static_cast<unsigned int>(glyph->advance.x)
        };
    }
    glBindTexture(GL_TEXTURE_2D, 0);

}
void DrawTextAt(GLuint progId, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    //shader.use();
    glUseProgram(progId);
    glUniform3f(glGetUniformLocation(progId, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (auto c : text)
    {
        Character ch = Characters[c];
        //ch.Bearing.xy = {glyph->bitmap_left, glyph->bitmap_top}
        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


/*
class X42
{
    public:
    X42() { std::cout << "    -->X42()\n"; }
    X42(int x) { std::cout << "    -->X42(int)\n"; }
    X42(X42& a) { std::cout << "    -->X42(&)\n";  }
    X42(X42&& a) noexcept  { std::cout << "    -->X42(&&)\n";  if (&a == this) return; *this = std::move(a);}
    X42& operator = (X42&& a) noexcept { std::cout << "-->X42(&&)\n"; return *this; }
	~X42() { std::cout << "    -->~X42()\n"; }
};
std::ostream& operator<<(std::ostream& os, const X42& obj)
{
    os << "X42";
    return os;
}
class MyClass
{
	std::unique_ptr<X42> data;
public:
    MyClass() { std::cout << "    MyClass()\n"; data = std::make_unique<X42>(42); }
    MyClass(MyClass& a) { std::cout << "    MyClass(&)\n";  }
    MyClass(MyClass&& a) noexcept  { if (&a == this) return; std::cout << "    MyClass(&&)\n";  data = std::unique_ptr<X42> (a.data.release());}
    MyClass& operator = (MyClass&& a) noexcept { if (&a == this) return *this; std::cout << "    MyClass = &&\n"; return *this; }
    //~MyClass() { std::cout << "    ~MyClass()\n"; }
};
int main()
{
    {
        std::vector <MyClass> v;
        //v.reserve(5);
        for (int i = 0; i < 5; i++)
        {
            std::cout << "####  Push back " << i << "\n";
            //v.emplace_back();
            v.push_back({});
        }
        std::cout << "####  END " << "\n";
    }
    return 0;
} //*/

#define  aaaaa(a, b)
int main()
{
    aaaaa ("qwe");
    return 0;
	std::wcout << L"Hello, 你好, 日本" << std::flush<< std::endl;
    zap::Init(4, 6);
    zap::Window window(SCR_WIDTH, SCR_HEIGHT, "Hello Window");
    zap::InitGlad();

    // OpenGL state
    // ------------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //Shader shader(vertexShaderXText, fragmentShaderXText);
    zap::Mesh textMesh({});

    textMesh.SetVertexShaderSource(vertexShaderXText);
    textMesh.SetFragmentShaderSource(fragmentShaderXText);
    textMesh.Finish();


    std::array<unsigned int, 2> window_size = window.GetSize();
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    glm::mat4 projection = glm::ortho(0.0f, (float)window_size[0], 0.0f, (float)window_size[1]);

	textMesh.UseProgram();
    glUniformMatrix4fv(glGetUniformLocation(textMesh.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    //FT_Library ft;
    //FT_Face face;
    //assert(!FT_Init_FreeType(&ft));
	//assert(!FT_New_Face(ft, "C:/Windows/Fonts/arial.ttf", 0, &face));
    //zap::util::scope_guard sg_freetype([&face, &ft]() { FT_Done_Face(face); FT_Done_FreeType(ft); } );
    //
    //buildTextInfo(face);
    buildTextInfo();


    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render loop
    // -----------
    //zap::Text text("C:/Windows/Fonts/arial.ttf", "Text", window.GetSize());
    
    while (window.Open())
    {
        if (window.isKeyPressed(zap::Key::ESC))
            window.Close();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawTextAt(textMesh.GetProgram(), "T_his is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        DrawTextAt(textMesh.GetProgram(), "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

        //text.Draw();
		window.Update();
		window.Draw();
    }

    glfwTerminate();
    return 0;
}

// render line of text
// -------------------

#endif