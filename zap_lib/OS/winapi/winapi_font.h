#pragma once

#ifndef __WINAPI_FONT_H__
#define __WINAPI_FONT_H__
//there goes Windows specific implementation
#ifdef _MSC_VER

namespace zap::os::winapi
{
	//void GetWindowsSystemFontPath(wchar_t* out_path, size_t max_length) {};
	unsigned int GetWindowsSystemFontPath(wchar_t* out_path, size_t max_length);
	unsigned int GetWindowsSystemFontPath(char* out_path, size_t max_length);
}

#endif // _MSC_VER
#endif // __WINAPI_FONT_H__
