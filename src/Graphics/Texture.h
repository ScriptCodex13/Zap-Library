#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Util/Message.h"
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

	class Texture
	{
	public:
		Texture(unsigned int external_id, const std::string path, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		Texture(unsigned int external_id, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilters filter = TextureFilters::LINEAR, MipmapSettings settings = MipmapSettings::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void deleteTexture();
		void genTexture();
		void bind();
		inline unsigned int getID() const { return i_TextureId; }
		inline unsigned int getHash() const { return i_ExternalId; }
	private:
		void genTextureFromFile();
		void genTextureFromData();
		unsigned int i_TextureId;
		unsigned int i_ExternalId;
	private:

		TextureFilters i_filter    = TextureFilters::LINEAR;
		MipmapSettings i_settings  = MipmapSettings::LINEAR_MIPMAP_LINEAR;
		TextureWrapping i_wrapping = TextureWrapping::CLAMP_TO_BORDER;

		std::string i_path;

		int i_width;
		int i_height;
		int i_nrChannels;

		unsigned char* i_TextureData = nullptr;

		GLenum i_Type;

	};

};



#endif

