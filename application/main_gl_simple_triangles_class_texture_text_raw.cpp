#include "enabler.h"
#ifdef SIMPLE_TRIANGLES_CLASS_TEXTURE_TEXT_RAW_CPP

#define _USE_MATH_DEFINES

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include <fstream>
//freetype / ftglyph.h

#include <cassert>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <map>

using namespace std;



//SQL like in function
//SQL like in: "... val in (arg1, arg2...)" in C++ to be "... in (val, arg1, arg2...)"
template<typename T>
inline bool in(T val, T arg)
{
    return val == arg;
}
template <typename T, typename ... Ts>
inline bool in(T val, T arg, Ts ... args)
{
    if (val == arg) return true;
    return in(val, args...);
}
//SQL like in, required for compile time expression and static asserts
template<typename T> constexpr
inline bool const_in(const T val, const T arg)
{
    return val == arg;
}
template <typename T, typename ... Ts> constexpr
inline bool const_in(const T val, const T arg, const Ts ... args)
{
    if (val == arg) return true;
    return const_in(val, args...);
}
//Oracle SQL like decode:   decode (initial, from1, to1, from2, to2, from3 to3, default) //with default provided
//Oracle SQL like decode:   decode (initial, from1, to1, from2, to2, from3 to3)          //with no default provided
template<typename T> T decode(T initial) { return initial; } //default value not provided
template<typename T> T decode(T initial, T b) { return b; }  //if default value provided
template<typename T, typename ... Ts> T decode(T initial, T decode_from, T decode_to, Ts... args)
{
    if (initial == decode_from) return decode_to;
    return decode<T>(initial, args ...);
}


//buffer views
//Unlike std::array_view, the size is not required to be known at compile time
//Are attached to external buffer to Provide 1D, 2D indexed access to contiguous buffers
//These views to not manage the internal buffer, these are projections only
//Buffer lifetime is managed externally only.
template <typename T> class buffer_view //1D
{
    T* buffer; //buffer length is managed externally, buffer_view doesn't care about it
public:
    buffer_view() : buffer_view(nullptr) {}
    buffer_view(T* t) : buffer(t) {}
    void reset(T* t) { buffer = t; }    //for instance reuse purpose
    T* const get() { return buffer; } //for debugging purposes, generally useless
    T& operator [](size_t i) { return buffer[i]; }
    T* operator + (size_t i) { return buffer + i; }
    void read(buffer_view& src_view, size_t lenght)
    {
        for (int i = 0; i < lenght; i++)
            buffer[i] = src_view[i];
    }
};
template <typename T, int alignment = 1> class buffer_view2D
{
    static_assert (const_in(alignment, 1, 2, 4, 8, 16), "Alignment template value can take only values 1, 2, 4, 8, 16");
    size_t width; //length of lines, can't change and must be kept internally
    T* buffer;
    size_t align(size_t w)
    {
        if ((w == 1) || !(w % alignment))
            return w;
        return w - (w % alignment) + alignment;
    }
public:
    using type = T;
    buffer_view2D() : buffer(nullptr), width(0) {}
    buffer_view2D(T* ptr, size_t w) { reset(ptr, w); }
    void reset(T* ptr, size_t w) { buffer = ptr; width = align(w); } //for instance reuse purpose
    inline size_t get_width() const { return width; }
    T* get() const { return buffer; } //for debugging purposes, generally useless
    //line number
    inline buffer_view<T> operator [] (size_t i)
    {
        return buffer_view(buffer + width * i);
    }

    inline void read(buffer_view2D& src_view, size_t rows)
    {
        assert(src_view.width >= this->width && "buffer overflow");
        for (int j = 0; j < rows; j++)
            this->operator[](j).read(src_view[j], this->get_width());
    }

    //inverts the lines order
    //useful only in context of generating textures
    //any other reverses are easily implemented outside
    inline void reverse_rows_read(buffer_view2D& src_view, size_t rows)
    {
        assert(src_view.width >= this->width && "buffer overflow");
        for (int j = 0, i = rows - 1; j < rows; j++, i--)
            this->operator[](j).read(src_view[i], this->get_width());
    }
};

//realloc vector in a way the memory allocated never schrinks
//this is intended to reduce number of memory allocations
template<class T> void vector_realloc(std::vector<T>& vt, size_t len)
{
    if (vt.capacity() < len)
        vt.reserve(len * 2);
    vt.resize(len);
}
//realloc vector and reset content 
template<class T> void vector_realloc(std::vector<T>& vt, size_t len, T resetValue)
{
    vector_realloc(vt, len);
    std::fill(vt.begin(), vt.end(), 0x00);
}


template<typename Deleter> class scope_guard
{
    Deleter t;
public:
    scope_guard(Deleter _t) :t(_t) {}
    ~scope_guard() { t(); }
};




class FreeType
{
    FT_Library ft_instance = nullptr;
    FT_Face i_font = nullptr;
public:
    FreeType() { FT_Init_FreeType(&ft_instance); }
    ~FreeType() { free(); }
    void free() //to call outside destructor if needed
    {
        if (i_font) FT_Done_Face(i_font);
        if (ft_instance) FT_Done_FreeType(ft_instance);
        ft_instance = nullptr;
        i_font = nullptr;
    }

    FT_Library getLibrary() { return ft_instance; }
    FT_Face getFace() { return i_font; }
    void LoadFont(const std::string font_path) {
       
        FT_New_Face(ft_instance, font_path.c_str(), 0, &i_font);
    }

} freetype;

const char* vertexShaderSource = R"glsl(#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
//layout (binding = 0) uniform vec3 passaround; //binding can't be applied for simple uniforms
void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
})glsl";

const char* fragmentShaderSource = R"glsl(#version 420 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
    float redVal = texture(ourTexture, TexCoord).r;
    FragColor = vec4(redVal, redVal, redVal, 1);
})glsl";

//
//
class CharInfo
{
public:
    size_t width = 0, height = 0, top = 0, advance_x = 0;
    std::vector <unsigned char> buffer{};
public:
    CharInfo() {}
    CharInfo(size_t _width, size_t _height, size_t _top, size_t _advance_x, unsigned char* buf) :
        width(_width), height(_height), top(_top), advance_x(_advance_x)
    {
        buffer.resize(width * height);
        memcpy(buffer.data(), buf, width * height);
    }
};
//cache
std::map<wchar_t, CharInfo> cachedChars;



bool drawGlythBitmapFromCache(buffer_view2D<unsigned char> target_view, size_t& pen_x, size_t& pen_y, wchar_t c, int fontsize)
{
    if (cachedChars.count(c) == 0)return false;//this char is not cached
    CharInfo& cached = cachedChars[c];
    int ytop = pen_y + cached.top;

    buffer_view2D<unsigned char> char_buf_view(cached.buffer.data(), cached.width);
    for (size_t i = 0, xi = pen_x; i < cached.width; i++, xi++)
    {
        for (size_t j = 0; j < cached.height; j++)
        {
            char pixel_val = char_buf_view[j][i];
            //assert(pos < bufsize);
            target_view[ytop + j][pen_x + i] = pixel_val;
        }
    }
    pen_x += cached.advance_x;

    return true; //this char is fully loaded from cache

}
void drawGlythBitmap(buffer_view2D<unsigned char> target_view, size_t& pen_x, size_t& pen_y, wchar_t c, int fontsize)
{
    if (drawGlythBitmapFromCache(target_view, pen_x, pen_y, c,  fontsize))
        return;
    //wcout << "compute char: " << c << endl;
    FT_Face face = freetype.getFace();
    FT_Load_Char(face, c, FT_LOAD_RENDER);

    FT_GlyphSlot& glyph = face->glyph;
    FT_Bitmap& btm = glyph->bitmap;
    FT_Bitmap& pbtm = glyph->bitmap;
    size_t top = (glyph->metrics.vertBearingY >> 6); //Works for MSGothic, doesn't work for Arial
    //Works for Arial, doesn't work for MSGothic
    if (!FT_HAS_VERTICAL(freetype.getFace()))
        top = fontsize - glyph->bitmap_top + (face->bbox.yMin >> 6);

    int ytop = pen_y + top;

    int total = std::min<int>(pbtm.rows, fontsize);
    size_t advance_x = glyph->advance.x >> 6;

    cachedChars.emplace(c, CharInfo{ pbtm.width, pbtm.rows, top, advance_x, (unsigned char*)pbtm.buffer }); 

    //Read it from cache anyway, avoid redundant routines
    bool ret = drawGlythBitmapFromCache(target_view, pen_x, pen_y, c, fontsize);
    assert(ret && "character c must exist in ret");
}
void drawString3TIntoBitman(buffer_view2D<unsigned char> buf, const wchar_t* str, size_t& outer_width, size_t& pen_y, int fontsize)
{
    size_t len = wcslen(str);
    for (size_t i = 0; i < wcslen(str); i++)
        drawGlythBitmap(buf, outer_width, pen_y, str[i], fontsize);
}

std::vector<unsigned char> texture_data_target;
std::vector<unsigned char> texture_data_source;

void drawString3TIntoBitman(const wchar_t* str, unsigned int fontSizeFT, size_t& outer_width, size_t& pen_y)
{
    size_t src_width = fontSizeFT * wcslen(str) * 2; //make it twice as wide as widest possible two chars
    size_t src_size = fontSizeFT * src_width; //2D array, fontSizeFT rows, tg_width columns

    //never schrink, so reduce the memory reallocation occurences
    vector_realloc<unsigned char>(texture_data_source, src_size, 0x00);

    size_t tg_width = 0, tg_height = 0;
    FT_Set_Pixel_Sizes(freetype.getFace(), 0, fontSizeFT);
    drawString3TIntoBitman(buffer_view2D(texture_data_source.data(), src_width), str, tg_width, tg_height, fontSizeFT);

    size_t tg_size = tg_width * fontSizeFT; //2D array, fontSizeFT rows, tg_width columns
    //never schrink, so reduce the memory reallocation occurences
    vector_realloc<unsigned char>(texture_data_target, src_size, 0x00);

    buffer_view2D src_view = buffer_view2D(texture_data_source.data(), src_width),
        tg_view = buffer_view2D(texture_data_target.data(), tg_width);
    tg_view.reverse_rows_read(src_view, fontSizeFT); //target reads fontSizeFT lines from source

    outer_width += tg_width;
}
unsigned int loadTextureFromData(int width, int height, void* pTextureData)
{
    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //GLfloat border_color[] = { 1.0, 0.0,  0.0, 1.0};
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);
    /////////////////////////////////////
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
}
//unsigned int loadTextureFromBuffer(int width, int height, GLuint textureBuffer)
//{
//    unsigned int textureID = 0;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    //GLfloat border_color[] = { 1.0, 0.0,  0.0, 1.0};
//    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);
//    /////////////////////////////////////
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);// pTextureData);
//    glGenerateMipmap(GL_TEXTURE_2D);
//    return textureID;
//}

unsigned int loadTextureFromData2(int width, int height, void* pTextureData, GLint alignment = 1)
{
    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //GLfloat border_color[] = { 1.0, 0.0,  0.0, 1.0};
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);
    /////////////////////////////////////
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint currentAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &currentAlignment);
    //4 is the default for GL_UNPACK_ALIGNMENT, probably for a good reason
    //Make sure it is not changed somewhere, so different parts of program do not interfere here
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    scope_guard restoreAlignment([currentAlignment]() {glPixelStorei(GL_UNPACK_ALIGNMENT, currentAlignment); });

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    return textureID;
}
void flushData(unsigned int textureID, int width, int height, void* pTextureData)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void flushSubData(unsigned int textureID, GLint xoffset, GLint yoffset, int width, int height, void* pTextureData)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void flushData2(unsigned int textureID, int width, int height, void* pTextureData, GLint alignment = 1)
{
    GLint currentAlignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &currentAlignment);
    //4 is the default for GL_UNPACK_ALIGNMENT, probably for a good reason
    //Make sure it is not changed somewhere, so different parts of program do not interfere here
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    scope_guard restoreAlignment([currentAlignment]() {glPixelStorei(GL_UNPACK_ALIGNMENT, currentAlignment); });

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

//
void dumpTextureBuffer(const unsigned char* buf, size_t cols, size_t lines) //cols ie:x/width/    lines ie:y/heigth
{
    ////dump texture to file;
    std::ofstream dump_content;
    dump_content.open("dump_texture.txt");
    buffer_view2D dumper_view(const_cast<unsigned char*>(buf), cols);
    for (int j = 0; j < lines; j++)
    {
        for (size_t i = 0; i < dumper_view.get_width(); i++)
            dump_content << decode<unsigned char>(dumper_view[j][i], 0, ' ', 'W');
        dump_content << std::endl;
    }
    dump_content << std::flush;
    dump_content.close();
}


unsigned int LoadTexture(const char* fileName, int RGBType)
{
    //generate texture in gfx card and get its ID
    unsigned int textureID1 = 0;
    glGenTextures(1, &textureID1);
    int width, height, numberOfChannels;
    unsigned char* imageData = stbi_load(fileName, &width, &height, &numberOfChannels, 0);
    scope_guard freeTextureData([imageData]() {if (imageData) stbi_image_free(imageData); });
    // TODO: report failure and end program
    if (!imageData) return textureID1;

    //bind this texture to make it the current one
    glBindTexture(GL_TEXTURE_2D, textureID1);

    //WRAPPING OPTIONS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);        //s = horizonal axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //t = vertical axis
    //FILTERING OPTIONS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //on shrink
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//on stretch

    //these vars get filled in when image is loaded

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, RGBType, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID1;
}


GLFWwindow* appInit() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    return window;
}

GLuint buildShaders(const char* vs, const char* fs)
{

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);

    int ok;
    char info[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &ok);

    if (!ok)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, info);
        cerr << "Failed to compile Vertex Shader: Mesh.cpp/void ez::Mesh2D::Finish()" << std::endl;
        std::cerr << info << std::endl;
    }
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &ok);

    if (!ok)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        std::cerr << "Failed to compile Fragment Shader:" << std::endl;
        std::cerr << info << std::endl;
    }

    //
    //Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ok);

    glLinkProgram(shaderProgram);
    if (!ok)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, info);
        std::cerr << "Failed to link Shader Program:" << std::endl;
        std::cerr << info << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}



struct geometry
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
}   geometry_text{
            {
                // positions         //// colors         // texture coords          
                 0.9f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
                 0.9f,  0.7f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right bottom
                 0.1f,  0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left bottom
                 0.1f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left top     And also this
            },
            {
                0, 3, 1, //right/top     right/bottom left/top
                1, 3, 2  //right/bottom  left/bottom  left/top
            }
        },
    geometry_plane{
            {
                // positions         //// colors         // texture coords          
                -0.1f,  0.9f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
                -0.1f,  0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right bottom
                -0.9f,  0.2f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left bottom
                -0.9f,  0.9f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left top     And also this
            },
            {
                0, 3, 1, //right/top     right/bottom left/top
                1, 3, 2  //right/bottom  left/bottom  left/top
            }
        },
    geometry_from_data{
            {
                // positions         //// colors         // texture coords          
                -0.1f, -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
                -0.1f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right bottom
                -0.9f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left bottom
                -0.9f, -0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left top     And also this
            },
            {
                0, 3, 1, //right/top     right/bottom left/top
                1, 3, 2  //right/bottom  left/bottom  left/top
            }
        },
    geometry_from_buffer{
            {
                // positions         //// colors         // texture coords          
                 0.9f, -0.1f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // right top
                 0.9f, -0.8f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right bottom
                 0.1f, -0.8f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // left bottom
                 0.1f, -0.1f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // left top     And also this
            },
            {
                0, 3, 1, //right/top     right/bottom left/top
                1, 3, 2  //right/bottom  left/bottom  left/top
            }
    };


unsigned int vertexBuffer;
GLuint createVao(const geometry& geo)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, geo.vertices.size() * sizeof(float), geo.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int ebo = -1;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.indices.size() * sizeof(int), geo.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return vao;
}

int main()
{
    freetype.LoadFont("C:/Windows/Fonts/arial.ttf");

    GLFWwindow* window = appInit();

    stbi_set_flip_vertically_on_load(true);
    GLuint textureIDPlane = LoadTexture("./textures/learnopengl/airplane.png", GL_RGBA);

    unsigned char x8x8[] = {
        255,   0,   0,   0,   0,   0,   0,   0,
          0, 255,   0,   0,   0,   0,   0,   0,
          0,   0, 255,   0,   0,   0,   0,   0,
          0,   0,   0, 255,   0,   0,   0,   0,
          0,   0,   0,   0, 255,   0,   0,   0,
          0,   0,   0,   0,   0, 255,   0,   0,
          0,   0,   0,   0,   0,   0, 255,   0,
          0,   0,   0,   0,   0,   0,   0, 255
    };
    unsigned char x3x3[] //aligned to 4
    {
          0,   0, 255, 0,
        255,   0,   0, 0,
          0, 255,   0, 0,
    };
    unsigned char x1a[] = {255};
    unsigned char x2a[] = {255, 0, 0, 255};

    GLuint textureIDFromData = loadTextureFromData(8, 8, x8x8);
    //flushSubData(textureIDFromData, 4, 1, 3, 3, x3x3);

    //GLuint textureIDText = loadTextureFromData2(2, 2, x2a);
    unsigned int fontSizeFT = 48;
    size_t outer_width = 0,  pen_y = 0;
    drawString3TIntoBitman(L"Heglo", fontSizeFT, outer_width, pen_y);
    GLuint textureIDText = loadTextureFromData2(outer_width, fontSizeFT, texture_data_target.data());
    GLuint shaderProgram = buildShaders (vertexShaderSource, fragmentShaderSource);
    //Create VAOs for different textures
    GLuint vao_text        = createVao(geometry_text);
    GLuint vao_plane       = createVao(geometry_plane);
    GLuint vao_from_data   = createVao(geometry_from_data);
    GLuint vao_from_buffer = createVao(geometry_from_buffer);

    GLuint textureBuffer, textureBufferId;
    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_TEXTURE_BUFFER, textureBuffer);
    glBufferData(GL_TEXTURE_BUFFER, sizeof(x8x8) * sizeof(unsigned char*), x8x8, GL_STATIC_DRAW);
    glGenTextures(1, &textureBufferId);
    glBindTexture(GL_TEXTURE_BUFFER, textureBufferId);
    glTexBuffer(textureBufferId, GL_RED, textureBuffer);


    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    wchar_t text_out[1024];

    //std::ofstream dump_content;
    //dump_content.open("dump_texture.txt");
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        float time = glfwGetTime() / 8, whole, frac;
        frac = std::modf(time, &whole);
        swprintf(text_out, L"Hello texture %f texture hellO", frac);
        swprintf(text_out, L"Heglo %d", (int)(frac * 100.f));

        double currentTime = glfwGetTime();
        double expr = frac * M_PI * 2;
        x3x3[0] = (cos(expr) + 1) / 2 * 255.f;
        //wcout << x3x3[0] << " " << flush;
        //dump_content << (int)x3x3[0] << "\n";
        flushSubData(textureIDFromData, 4, 1, 3, 3, x3x3);

        //flushData(textureIDText, 1, 1, x1);
        //flushData(textureIDText, 2, 2, x2);
        //lushData2(textureIDText, 1, 1, x1a);
        //flushData2(textureIDText, 2, 2, x2a);
        outer_width = 0;
        drawString3TIntoBitman(text_out, fontSizeFT, outer_width, pen_y);
        flushData2(textureIDText, outer_width, fontSizeFT, texture_data_target.data());


        glClearColor(0.3f, 0.8f, 0.3f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
 
        glBindVertexArray(vao_text);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDText);
        glDrawElements(GL_TRIANGLES, geometry_text.indices.size(), GL_UNSIGNED_INT, 0);
 
        glBindVertexArray(vao_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDPlane);
        glDrawElements(GL_TRIANGLES, geometry_plane.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vao_from_data);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDFromData);
        glDrawElements(GL_TRIANGLES, geometry_from_data.indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(vao_from_buffer);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIDFromData);
        glDrawElements(GL_TRIANGLES, geometry_from_buffer.indices.size(), GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    //dump_content << std::flush;
    //dump_content.close();

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


#endif