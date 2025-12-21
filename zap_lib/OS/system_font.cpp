#include "system_font.h"

#ifdef _MSC_VER
#include "winapi/winapi_font.h"
#else //there to be added  .h/.cpp modules for other operation systems
static_assert(false, "no module implemented for this OS");
#endif

namespace zap::os
{
unsigned int GetSystemFontPath(wchar_t* fontFilePath, size_t max_length)
{
#ifdef _MSC_VER
	unsigned int ret = 0;
	ret = winapi::GetWindowsSystemFontPath(fontFilePath, max_length);
	return ret;
#else //there to be handled for other operation systems
	static_assert(false, "GetSystemFont not implemented for this OS");
#endif
}
unsigned int GetSystemFontPath(char* fontFilePath, size_t max_length)
{
#ifdef _MSC_VER
	unsigned int ret = 0;
	ret = winapi::GetWindowsSystemFontPath(fontFilePath, max_length);
	return ret;
#else //there to be handled for other operation systems
	static_assert(false, "GetSystemFont not implemented for this OS");
#endif
}
}