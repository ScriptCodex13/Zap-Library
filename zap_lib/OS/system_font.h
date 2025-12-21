#pragma once
#ifndef __OS_SYSTEM_FONT_H__
#define __OS_SYSTEM_FONT_H__

namespace zap::os
{
	unsigned int GetSystemFontPath(wchar_t* fontFilePath, size_t max_length);
	unsigned int GetSystemFontPath(char* fontFilePath, size_t max_length);
}

#endif