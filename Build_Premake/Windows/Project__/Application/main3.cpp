//#define ZAP_LIBRARY_MAIN3_CPP
//#include  "enabler.h"
#ifdef ZAP_LIBRARY_MAIN3_CPP

#include <atlbase.h>
#include <atlwin.h>
#include <iostream>
#include <Zap.h>
using std::cout;
using std::endl;

SIZE paintWithFont(HDC hDC, int x, int y, const wchar_t* str, wchar_t* font, int height = 0, int width = 0, DWORD iCharset = DEFAULT_CHARSET)
{
    HFONT hFont = ::CreateFontW(height, width, 0, 0, FW_DONTCARE,
        0, 0, 0, iCharset,
        0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, font);
    HGDIOBJ hOldFont = ::SelectObject(hDC, hFont);
    zap::util::scope_guard fnt_guard([&hFont, &hOldFont]() {::DeleteObject(hFont); ::DeleteObject(hOldFont); });
    SIZE sz;
    ::GetTextExtentPoint32W(hDC, str, wcslen(str), &sz);
    ::TextOutW(hDC, x, y, str, wcslen(str));
    ::SelectObject(hDC, hOldFont);
    return sz;
}
char* loadFileContentW(wchar_t* fileName)
{
    char* buf = 0;

    HBITMAP btRet = 0;

    HANDLE hFile = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE) return 0;
    zap::util::scope_guard file_guard([&hFile]() { CloseHandle(hFile); });
    DWORD dwFileSizeHigh;
    DWORD fileSize = GetFileSize(hFile, &dwFileSizeHigh);
    buf = (char*)HeapAlloc(GetProcessHeap(), 0, fileSize);
    DWORD read;
    if (!ReadFile(hFile, buf, fileSize, &read, 0))
    {
        HeapFree(GetProcessHeap(), 0, buf);
        buf = 0;
    }

    return buf;


}
HBITMAP loadImageFromFileW(wchar_t* fileName, SIZE* psz = 0)
{
    char* buf = loadFileContentW(fileName);
    if (!buf) return 0;
    zap::util::scope_guard file_guard([&buf]() { HeapFree(GetProcessHeap(), 0, buf); });

    HBITMAP btRet = 0;
    BITMAPFILEHEADER* bmpfh = (BITMAPFILEHEADER*)buf;
    BITMAPINFO* pbmInfo = (BITMAPINFO*)(buf + sizeof(BITMAPFILEHEADER));
    if (psz)
        *psz = { pbmInfo->bmiHeader.biWidth, pbmInfo->bmiHeader.biHeight };


    if (pbmInfo->bmiHeader.biCompression == BI_RGB)
    {
        HDC hdcSrc = CreateDCW(L"DISPLAY", 0, 0, 0);
        btRet = CreateDIBitmap(hdcSrc, &pbmInfo->bmiHeader, CBM_INIT, buf + bmpfh->bfOffBits, pbmInfo, DIB_RGB_COLORS);
        DeleteDC(hdcSrc);
        return btRet;
    }

    return btRet; //0;
}
void drawLoadImageFromFileW(HDC hdc, wchar_t* fileName, int x, int y, SIZE* psz = 0)
{
    HBITMAP hBitmapBmpFile = loadImageFromFileW(fileName);
    BITMAP bm;
    ::GetObject(hBitmapBmpFile, sizeof(bm), &bm);
    HDC hdcMem = CreateCompatibleDC(0);
    HGDIOBJ hbmOld = SelectObject(hdcMem, hBitmapBmpFile); //<-- select bitmap into memory DC
    BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
    if (psz)
        *psz = { bm.bmWidth , bm.bmHeight };
    DeleteObject(hBitmapBmpFile);
    DeleteDC(hdcMem);

}

HDC loadStringIntoDC(const wchar_t* str, SIZE* psz = 0)
{
    HBITMAP btRet = 0;
    HDC hdcSrc = CreateDCW(L"DISPLAY", 0, 0, 0);
    ::TextOutW(hdcSrc, 0, 0, str, wcslen(str));
    if (psz)
        ::GetTextExtentPoint32W(hdcSrc, str, wcslen(str), psz);
    return hdcSrc;
}
void drawLoadStringIntoDC(HDC hdc, const wchar_t* str, int x, int y, SIZE* psz = 0)
{
    SIZE sz;
    HDC hdcString = loadStringIntoDC(str, &sz);
    BitBlt(hdc, x, y, sz.cx, sz.cy, hdcString, 0, 0, SRCCOPY);
    if (psz)
        *psz = sz;
    DeleteDC(hdcString);
}
void drawSinusoid(HDC hdc, LONG right)
{
    for (double x = 0, xbase = 120; x < right; x += 0.1)
    {
        int y = ((double)40 * sin((double)x / 20) + xbase);
        SetPixel(hdc, x, y, RGB(256, 0, 0));
    }
}
int drawGlythBitmap2(FT_Face ftface, HDC hdc, int& pen_x, int& pen_y, wchar_t c)
{
    FT_Load_Char(ftface, c, FT_LOAD_RENDER);
	//ABOVE is equivalent to:
    //unsigned int glyph_index = FT_Get_Char_Index(ftface, c);
    //FT_Load_Glyph( ftface, glyph_index, FT_LOAD_RENDER);

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
	int fullHeigth = (ftface->size->metrics.y_ppem);
	fullHeigth = ytop + pbtm.rows;
    int asc  = int(ftface->ascender * (ftface->size->metrics.y_scale / 65536.0)) >> 6;
    int desc = int(abs(ftface->descender * (ftface->size->metrics.y_scale / 65536.0))) >> 6;
	return fullHeigth;
}
void drawStringFT_IntoDC(FT_Face ftface, HDC hdc, const wchar_t* str, int& pen_x, int& pen_y, SIZE* sz = 0)
{
    for (int i = 0; str[i]; i++)
        drawGlythBitmap2(ftface, hdc, pen_x, pen_y, str[i]);
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
		zap::util::scope_guard sg_ps([this, &ps]() { EndPaint(&ps); });

        int xstart = 50, pen_x = 50, pen_y = 200, ystart = 200, step = 30;

		std::wstring str = L"HȚ_țÂâÎĂ,你好, 日本egllo ";
		//str = L"Hello, 你好, 日本";
		wprintf(L"%s\n", str.c_str());

        SIZE sz;
        unsigned int fontSize = 48;
        drawStringFT_IntoDC(ftface, hdc, str.c_str(), fontSize, pen_x, pen_y);

        drawLoadImageFromFileW(hdc, L".\\bmp1.bmp", 10, 50);
        drawLoadStringIntoDC(hdc, L"string: .\\bmp1.bmp", 10, 10);  drawLoadStringIntoDC(hdc, str.c_str(), 200, 10);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        //Rectangle(hdc, xstart, ystart, xstart + 500, ystart + 48); // <- will be 'transparent'/unfilled 
        Rectangle(hdc, xstart, ystart, pen_x, ystart + 48); // <- will be 'transparent'/unfilled 

        ystart = 300; xstart = 50;
		paintWithFont(hdc, xstart, ystart, str.c_str(), L"MingLiU", 25);
        //SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, xstart, ystart, xstart + 500, ystart + 25); ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"Arial"); ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"Times New Roman"); ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"Tahoma"); ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"MS Ming Cho"); ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"MS UI Gothic"); ystart += step;
        sz = paintWithFont(hdc, 50, ystart, str.c_str(), L"HGPSoeiKakupoptai", 27);
        Rectangle(hdc, xstart, ystart, xstart + sz.cx, ystart + sz.cy);

        ystart += step;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"FangSong"); ystart += step;
        sz = paintWithFont(hdc, 50, ystart, L"saaaauuu aaaa ooo", L"FangSong");
        Rectangle(hdc, xstart, ystart, xstart + sz.cx, ystart + sz.cy);
        ystart += step;

		drawSinusoid(hdc, ps.rcPaint.right);
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

template < typename T> class buffer_2d
{
    int width, height;
    //std_vector<T> buf;
};



template <typename T> struct long_t{};
template <> struct long_t<char> {using t = short;};
template <> struct long_t<unsigned char> { using t = wchar_t; };
template <> struct long_t<wchar_t> { using t = unsigned int; };
template <> struct long_t<short> { using t = int; };
template <> struct long_t<unsigned short> { using t = unsigned int; };
template <> struct long_t<int> { using t = long long; };
template <> struct long_t<long> { using t = long long; };
template <> struct long_t<unsigned int> { using t = unsigned long long; };
template <> struct long_t<unsigned long> { using t = unsigned long long; };
template <> struct long_t<float> { using t = double; };

template <typename T> constexpr bool is_signed()
{
    return (T(-1) >> 1 == T(-1));
};
template <typename T> constexpr bool sign_carried()
{
    return is_signed<T>() == is_signed<long_t<T>::t>();
};
template <> constexpr bool sign_carried<float>()
{
    return true;
};
template <typename T> constexpr bool has_long()
{
    return sizeof(T) * 2 == sizeof(long_t<T>::t);
};



static_assert (sign_carried<char>(), "long_t<char> does not keep the sign of char");
static_assert (sign_carried<unsigned char>(), "long_t<unsigned char> does not keep the sign of unsigned char");
static_assert (sign_carried<wchar_t>(), "long_t<wchar_t> does not keep the sign of wchar_t");
static_assert (sign_carried<short>(), "long_t<short> does not keep the sign of short of short");
static_assert (sign_carried<unsigned short>(), "long_t<unsigned short> does not keep the sign of unsigned short");
static_assert (sign_carried<int>(), "long_t<int> does not keep the sign of int");
static_assert (sign_carried<long>(), "long_t<long> does not keep the sign of long");
static_assert (sign_carried<unsigned int>(), "long_t<unsigned int> does not keep the sign of unsigned int");
static_assert (sign_carried<unsigned long>(), "long_t<unsigned long> does not keep the sign of unsigned long");
static_assert (sign_carried<float>(), "long_t<float> does not keep the sign of float");


static_assert (has_long<char>(), "long_t<char> not provided or size not doubled");
static_assert (has_long<wchar_t>(), "long_t<wchar_t> not provided or size not doubled");
static_assert (has_long<unsigned char>(), "long_t<unsigned char> not provided or size not doubled");
static_assert (has_long<short>(), "long_t<short> not provided or size not doubled");
static_assert (has_long<unsigned short>(), "long_t<unsigned short> not provided or size not doubled");
static_assert (has_long<int>(), "long_t<int> not provided or size not doubled");
static_assert (has_long<long>(), "long_t<long> not provided or size not doubled");
static_assert (has_long<unsigned int>(), "long_t<unsigned int> not provided or size not doubled");
static_assert (has_long<unsigned long>(), "long_t<unsigned long> not provided or size not doubled");
static_assert (has_long<float>(), "long_t<float> not provided or size not doubled");


template <typename T, typename ... Ts, typename R=long_t<typename T>::t> R constexpr ssum(const Ts&... args)
{
    return (R(T(args)) + ...);
}

int main()
{
	long_t<char>::t sx = ssum<char>(255, 255, 255);
    long_t<unsigned char>::t usx = ssum<unsigned char>(255, 255, 255);
	cout << "sx=" << sx << " usx=" << usx << endl;
    signed char aaa = static_cast<signed char>(255) + static_cast<signed char>(255)+ static_cast<signed char>(255);
	cout << "aaa=" << int(aaa) << endl;
    return 0;

	cout << "wchar_t " << (is_signed<wchar_t> () ? "is signed" :"is unsigned")  << endl;
	cout << "short " << (is_signed<short> () ? "is signed" :"is unsigned")  << endl;
	cout << "int " << (is_signed<int> () ? "is signed" :"is unsigned")  << endl;
	cout << "unsigned int " << (is_signed<unsigned int> () ? "is signed" :"is unsigned")  << endl;

	unsigned short x = 0xFF00, xx = x*2;
	long_t<unsigned short>::t y = x * 2;
	cout << "x=" << x << " xx=" << xx << " y=" << y << endl;
    return 0;

	long_t<int>::t a = 0;
	cout << sizeof(char) << ":"<< sizeof(long_t<char>::t) << endl;
	cout << sizeof(short) << ":"<< sizeof(long_t<short>::t) << endl;
	cout << sizeof(int) << ":"<< sizeof(long_t<int>::t) << endl;
	cout << sizeof(float) << ":" << sizeof(long_t<float>::t) << endl;
    return 0;
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