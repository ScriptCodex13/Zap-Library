#pragma once

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

// ToDo make a GUI part of the library later on and move  

// includes

#include "Mesh.h"

#include <string>

//

// ToDo add function to load a font in the loader or here

namespace zap
{
	// Text class will be here

	class Text
	{
	public:
		Text(const std::string text);
		~Text();

		void SetString();
		void SetSize();
		void SetPosition();
		void SetColor();

		void Draw();
	private:
		std::string i_content;

	private:
		// Functions to abstract text initialization here
	};
}

#endif