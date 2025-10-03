//#define ZAP_LIBRARY_MAIN3WINAPI_CPP
#ifdef ZAP_LIBRARY_WINAPI_CPP

#include <atlbase.h>
#include <atlwin.h>
#include <iostream>


using std::cout;
using std::endl;
template<typename T> class scope_guard
{
    T t;
public:
    scope_guard(T _t) :t(_t) {}
    ~scope_guard() { t(); }
};
SIZE paintWithFont(HDC hDC, int x, int y, const wchar_t* str, wchar_t* font, int height = 0, int width = 0, DWORD iCharset = DEFAULT_CHARSET)
{
    HFONT hFont = ::CreateFontW(height, width, 0, 0, FW_DONTCARE,
        0, 0, 0, iCharset,
        0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, font);
    HGDIOBJ hOldFont = ::SelectObject(hDC, hFont);
    scope_guard fnt_guard([&hFont, &hOldFont]() {::DeleteObject(hFont); ::DeleteObject(hOldFont); });
    SIZE sz;
    ::GetTextExtentPoint32W(hDC, str, wcslen(str), &sz);
    ::TextOutW(hDC, x, y, str, wcslen(str));
    ::SelectObject(hDC, hOldFont);
    return sz;
}


class CDemoWindow : public CWindowImpl<CDemoWindow>
{

public:

    CDemoWindow() 
    {
    }
    virtual ~CDemoWindow() {
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

		std::wstring str = L"HeglloÂȚâțÎĂ_, 你好, 日本";
		str = L"Hello, 你好, 日本";
		wprintf(L"%s\n", str.c_str());

        SIZE sz;
        unsigned int fontSize = 48;
        paintWithFont(hdc, 50, ystart, str.c_str(), L"Arial", fontSize); ystart += step;

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