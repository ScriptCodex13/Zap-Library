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
#include <fstream>


#include <cassert>
#include <cstdio>
#include <cstring>
#include <cmath>


using namespace std;



template <typename T> class buffer_view //1D
{
    T* printf_buffer; //buffer length is managed externally, buffer_view doesn't care about it
public:
    buffer_view() : buffer_view(nullptr) {}
    buffer_view(T* t) : printf_buffer(t) {}
    void reset(T* t) { printf_buffer = t; }    //for instance reuse purpose
    T* const get() { return printf_buffer; } //for debugging purposes, generally useless
    T& operator [](size_t i) { return printf_buffer[i]; }
    T* operator + (size_t i) { return printf_buffer + i; }
    void read(buffer_view& src_view, size_t lenght)
    {
        for (int i = 0; i < lenght; i++)
            printf_buffer[i] = src_view[i];
    }
};

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
template<int alignment = 1>size_t align(size_t w)
{
    static_assert (const_in(alignment, 1, 2, 4, 8, 16), "Alignment template value can take only values 1, 2, 4, 8, 16");
    if ((w == 1) || !(w % alignment))
        return w;
    return w - (w % alignment) + alignment;
}
template <typename T, int alignment = 1> class buffer_view2D
{
    static_assert (const_in(alignment, 1, 2, 4, 8, 16), "Alignment template value can take only values 1, 2, 4, 8, 16");
    size_t width; //length of lines, can't change and must be kept internally
    T* printf_buffer;
    size_t align(size_t w)
    {
        if ((w == 1) || !(w % alignment))
            return w;
        return w - (w % alignment) + alignment;
    }
public:
    using type = T;
    buffer_view2D() : printf_buffer(nullptr), width(0) {}
    buffer_view2D(T* ptr, size_t w) { reset(ptr, w); }
    void reset(T* ptr, size_t w) { printf_buffer = ptr; width = align (w); } //for instance reuse purpose
    inline size_t get_width() const { return width; }
    T* get() const { return printf_buffer; } //for debugging purposes, generally useless
    //line number
    inline buffer_view<T> operator [] (size_t i)
    {
        return buffer_view<T>(printf_buffer + width * i);
    }

    inline void read(buffer_view2D& src_view, size_t rows)
    {
        assert(src_view.width >= this->width && "buffer overflow");
        for (int j = 0; j < rows; j++)
            this->operator[](j).read(src_view[j], this->get_width());
    }
    //inverts the lines order
    inline void reverse_rows_read(buffer_view2D& src_view, size_t rows)
    {
        assert(src_view.width >= this->width && "buffer overflow");
        for (int j = 0, i = rows - 1; j < rows; j++, i--)
            this->operator[](j).read(src_view[i], this->get_width());
    }
};
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
    //FT_Error loadChar(FT_Face face, wchar_t c)
    //{
    //    return FT_Load_Char(face, c, FT_LOAD_RENDER);
    //    //FT_New_Face(ft_instance, "C:/Windows/Fonts/arial.ttf", 0, &i_font);
    //    //pface = std::make_unique<FT_Face, FT_Done_Face>(i_font);
    //    //uptr = std::make_unique < FT_Face, std::function<FT_Done_Face> > (i_font);
    //}
} freetype;


const char* vertexShaderSource = R"glsl(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;
out vec2 TexCoord;
void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
})glsl";

const char* fragmentShaderSource = R"glsl(#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = vec4(texture(ourTexture, TexCoord).r, texture(ourTexture, TexCoord).r, texture(ourTexture, TexCoord).r, 1);
})glsl";

//
//
void drawGlythBitmap(buffer_view2D<unsigned char> target_view, int& pen_x, int& pen_y, wchar_t c, size_t bufsize)
{
    FT_Load_Char(freetype.getFace(), c, FT_LOAD_RENDER);
    //assert (FT_HAS_VERTICAL(freetype.getFace())); //otherwise need to guess (letter g) //handle it later

    FT_GlyphSlot& glyph = freetype.getFace()->glyph;
    FT_Bitmap& btm = freetype.getFace()->glyph->bitmap;
    FT_Bitmap& pbtm = glyph->bitmap;
    int heigthAdvance = -glyph->bitmap_top + freetype.getFace()->size->metrics.y_ppem;
    int bearingy = glyph->bitmap_top, heigh = glyph->bitmap.rows;
    int ytop = pen_y + (glyph->metrics.vertBearingY >> 6); //the best so far, valid for FT_HAS_VERTICAL
    //if (!FT_HAS_VERTICAL(freetype.getFace())) ytop = pen_y + 48 - glyph->bitmap_top; // (glyph->bitmap.rows - glyph->bitmap_top);
    //glm::ivec2(i_font->glyph->bitmap.width, i_font->glyph->bitmap.rows), //Size
    //glm::ivec2(glyph->bitmap_left,  glyph->bitmap_top),                  //Bearing //ypos = y - (ch.Size.y - ch.Bearing.y)

    buffer_view2D char_buf_view(pbtm.printf_buffer, pbtm.width);
    for (int i = 0, xi = pen_x; i < pbtm.width; i++, xi++)
    {
        for (int j = 0; j < pbtm.rows; j++)
        {
            char pixel_val = char_buf_view[j][i];
            target_view[ytop + j][pen_x + i] = pixel_val;
        }
    }
    pen_x += glyph->advance.x >> 6;
}
void drawString3TIntoBitman(buffer_view2D<unsigned char> buf, const wchar_t* str, int& outer_width, int& pen_y, size_t bufsize)
{
    size_t len = wcslen(str);
    for (size_t i = 0; i < wcslen(str); i++)
        drawGlythBitmap(buf, outer_width, pen_y, str[i], bufsize);
}

std::vector<unsigned char> texture_data_target;
std::vector<unsigned char> texture_data_source;

void drawString3TIntoBitman(const wchar_t* str, unsigned int fontSizeFT, int& outer_width, int& pen_y)
{
    size_t src_width = fontSizeFT * wcslen(str) * 2; //make it twice as wide as widest possible two chars
    size_t src_size = fontSizeFT * src_width; //2D array, fontSizeFT rows, tg_width columns

    //never schrink, so reduce the memory reallocation occurences
    if (texture_data_source.capacity() < src_size)
        texture_data_source.reserve(src_size);
    texture_data_source.resize(src_size);
    std::fill(texture_data_source.begin(), texture_data_source.end(), 0);

    int tg_width = 0, tg_height = 0;
    FT_Set_Pixel_Sizes(freetype.getFace(), 0, fontSizeFT);
    drawString3TIntoBitman(buffer_view2D(texture_data_source.data(), src_width), str, tg_width, tg_height, src_size);

    size_t tg_size = tg_width * fontSizeFT; //2D array, fontSizeFT rows, tg_width columns
    //never schrink, so reduce the memory reallocation occurences
    if (texture_data_target.capacity() < tg_size)
        texture_data_target.reserve(tg_size);
    texture_data_target.resize(src_size);
    std::fill(texture_data_target.begin(), texture_data_target.end(), 0);

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
unsigned int loadTextureFromData2(int width, int height, void* pTextureData)
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

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
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
void flushData2(unsigned int textureID, int width, int height, void* pTextureData)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pTextureData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

//Just like Oracle SQL decode
template<typename T> T decode(T initial) { return initial; } //default value not provided
template<typename T> T decode(T initial, T b) { return b; }  //if default value provided
template<typename T, typename ... Ts> T decode(T initial, T decode_from, T decode_to, Ts... args)
{
    if (initial == decode_from) return decode_to;
    return decode<T>(initial, args ...);
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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




std::vector<float> vertices =
{
    // positions         //// colors         // texture coords          
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // left  
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // right 
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // top   
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left     And also this
};
std::vector<unsigned int> indices =
{
    0, 1, 3,
    1, 2, 3
};
unsigned int vertexBuffer;
GLuint createVao()
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int ebo = -1;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    return vao;
}

int main()
{
    freetype.LoadFont("C:/Windows/Fonts/arial.ttf");

    GLFWwindow* window = appInit();

    stbi_set_flip_vertically_on_load(true);
    GLuint textureIDPlane = LoadTexture("./textures/airplane.png", GL_RGBA);

    //unsigned char x1[] = {0, 0, 0,    0, 255, 0,    0, 0, 0};
    unsigned char x1[] = {255, 0, 0, 0 };
    unsigned char x2[] = {255, 0, 0, 0,      0, 255, 0, 0};
    unsigned char x1a[] = {255};
    unsigned char x2a[] = {255, 0, 0, 255};

    //GLuint textureIDText = loadTextureFromData(2, 2, x2);
    //GLuint textureIDText = loadTextureFromData2(2, 2, x2a);
    unsigned int fontSizeFT = 48;
    int outer_width = 0,  pen_y = 0;
    drawString3TIntoBitman(L"Heglo", fontSizeFT, outer_width, pen_y);
    GLuint textureIDText = loadTextureFromData2(outer_width, fontSizeFT, texture_data_target.data());
    GLuint shaderProgram = buildShaders (vertexShaderSource, fragmentShaderSource);
    GLuint vao = createVao();


    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    wchar_t text_out[1024];
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        float time = glfwGetTime(), whole, frac;
        frac = std::modf(time, &whole);
        swprintf(text_out, L"Hello texture %f texture hellO", frac);
        swprintf(text_out, L"Heglo %d", (int)(frac * 100.f));
        frac = (int) (float) 255.f * frac;
        frac /= 255; //renorm to 255
        x1[0] = cos(M_2_PI * frac) * 255 * 5;
        x2[0] = cos(M_2_PI * frac) * 255 * 5;
        x1a[0] = cos(M_2_PI * frac) * 255 * 5;
        x2a[0] = cos(M_2_PI * frac) * 255 * 5;

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
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, textureIDPlane);
        glBindTexture(GL_TEXTURE_2D, textureIDText);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


#endif