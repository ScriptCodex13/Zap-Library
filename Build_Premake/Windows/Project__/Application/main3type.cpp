//#define ZAP_LIBRARY_MAIN33TYPE_CPP
#ifdef ZAP_LIBRARY_MAIN33TYPE_CPP

#include <atlbase.h>
#include <atlwin.h>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H 
using std::cout;
using std::endl;
template<typename T> class scope_guard
{
    T t;
public:
    scope_guard(T _t) :t(_t) {}
    ~scope_guard() { t(); }
};
void drawGlythBitmap(FT_Face ftface, HDC hdc, int& pen_x, int& pen_y, wchar_t c)
{
    FT_Load_Char(ftface, c, FT_LOAD_RENDER);

    FT_GlyphSlot& glyph = ftface->glyph;
    FT_Bitmap& btm = ftface->glyph->bitmap;
    FT_Bitmap& pbtm = glyph->bitmap;
    int heigthAdvance = -glyph->bitmap_top + ftface->size->metrics.y_ppem;
    //int heigthAdvance = -glyph->bitmap_top; //stays on top of rectangle and intersects it
    int ytop = pen_y + heigthAdvance; //stays on top of rectangle and intersects it

    for (int i = 0, xi = pen_x; i < pbtm.width; i++, xi++)
    {
        for (int j = 0; j < pbtm.rows; j++)
        {
            char pixel_val = pbtm.buffer[j * pbtm.width + i] ^ -1;
            SetPixel(hdc, xi, ytop + j, RGB(pixel_val, pixel_val, pixel_val)); //almost ok
        }
    }

    pen_x += glyph->advance.x >> 6;
}
void drawStringFT_IntoDC(FT_Face ftface, HDC hdc, const wchar_t* str, int& pen_x, int& pen_y, SIZE* sz = 0)
{
    for (int i = 0; str[i]; i++)
        drawGlythBitmap(ftface, hdc, pen_x, pen_y, str[i]);
}
void drawStringFT_IntoDC(FT_Face ftface, HDC hdc, const wchar_t* str, unsigned int fontSizeFT, int& pen_x, int& pen_y, SIZE* sz = 0)
{
    FT_Set_Pixel_Sizes(ftface, 0, fontSizeFT);
    drawStringFT_IntoDC(ftface, hdc, str, pen_x, pen_y, sz);
}


class CDemoWindow : public CWindowImpl<CDemoWindow>
{
    FT_Library ftlib;
    FT_Face ftface;

public:

    CDemoWindow()
    {
        FT_Init_FreeType(&ftlib);
        //FT_New_Face(ftlib, "C:/Windows/Fonts/arial.ttf", 0, &ftface);
        FT_New_Face(ftlib, "C:/Windows/Fonts/msgothic.ttc", 0, &ftface);
    }
    virtual ~CDemoWindow() {
        FT_Done_Face(ftface);
        FT_Done_FreeType(ftlib);
    }
    /////////////////////////// Message Handlers /////////////////////////////////
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 1;
    }

    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(&ps);
        scope_guard sg_ps([this, &ps]() { EndPaint(&ps); });

        int xstart = 50, pen_x = 50, pen_y = 200, ystart = 200, step = 30;

        std::wstring str = L"Hello, 你好, 日本";

        SIZE sz;
        unsigned int fontSizeFT = 48;
        drawStringFT_IntoDC(ftface, hdc, str.c_str(), fontSizeFT, pen_x, pen_y);

        bHandled = TRUE;
        return 0;
    }

    LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        using std::cout;
        using std::endl;
        Invalidate();
        return 0;
    }
    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            bHandled = TRUE;
            break;
        }
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        PostQuitMessage(0);
        return 1;
    }

    BEGIN_MSG_MAP(CDemoWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
        MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
    END_MSG_MAP()
};

int main()
{
    CDemoWindow cwnd;

    HWND hWnd = cwnd.Create(NULL, CWindow::rcDefault, L"Hello", WS_OVERLAPPEDWINDOW);
    //cwnd.ShowWindow(SW_SHOW);
    cwnd.ShowWindow(SW_SHOWNORMAL);
    cwnd.UpdateWindow();

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

#endif