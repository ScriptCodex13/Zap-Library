#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Message.h"
#include <glad/glad.h>

namespace zap
{

	enum class TextureFilters : GLint
	{
		NEAREST = GL_NEAREST,
		LINEAR  = GL_LINEAR
	};

	enum class MipmapSettings : GLint
	{
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST
	};

	enum class TextureWrapping : GLint
	{
		REPEAT          = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	struct Texture
	{
		Texture(unsigned int id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void genTexture();
		void bind();

		unsigned int i_texture;
		std::string i_path;

		TextureFilters i_filter    = TextureFilters::LINEAR;
		MipmapSettings i_settings  = MipmapSettings::LINEAR_MIPMAP_LINEAR;
		TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER;

		bool i_textureloaded = false;

		int i_width;
		int i_height;
		int i_nrChannels;

		bool i_usePNG = false;

		unsigned int i_id;
	};

};



#endif

