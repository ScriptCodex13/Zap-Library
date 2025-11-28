#pragma once 

// Loads Shader code files but maybe audio Files or something like that later on

#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <fstream>
#include <sstream>

#include "Message.h"

namespace zap
{
	std::string LoadShaderSourcefromFile(const std::string filepath);
}

#endif