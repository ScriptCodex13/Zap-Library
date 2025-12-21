#ifdef _MSC_VER

#include "winapi_font.h"
#include <windows.h>
//#include <cstring>
#include <cstdio>
#include <string>
namespace zap::os::winapi
{

unsigned int  GetWindowsSystemFontPath(wchar_t* out_path, size_t max_length)
{
	wchar_t buf[2048] = L"windir";
	UINT ret = ::GetWindowsDirectoryW(buf, 2048);
	swprintf(out_path, max_length, L"%s/Fonts/arial.ttf", buf);
	return ret;
}
unsigned int  GetWindowsSystemFontPath(char* out_path, size_t max_length)
{
	char buf[2048] = "windir";

	UINT ret = ::GetWindowsDirectoryA(buf, 2048);
	_snprintf_s(out_path, max_length, max_length, "%s/Fonts/arial.ttf", buf);
	return ret;
}

}
#endif