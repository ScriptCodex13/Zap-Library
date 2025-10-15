#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Util/Message.h"
#include <glad/glad.h>

#include <array>
namespace zap
{

	enum class TextureFilter : GLint
	{
		NEAREST = GL_NEAREST,
		LINEAR  = GL_LINEAR
	};

	enum class MipmapSetting : GLint
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

	struct TextureDescriptor
	{
		TextureFilter filter = TextureFilter::LINEAR;
		MipmapSetting setting = MipmapSetting::LINEAR_MIPMAP_LINEAR;
		TextureWrapping wrapping_s = TextureWrapping::CLAMP_TO_BORDER;
		TextureWrapping wrapping_t = TextureWrapping::CLAMP_TO_BORDER;

		GLenum type; //GL_RED GL_RGB GL_RGBA...
		int width;
		int height;
		//int nrChannels;

		bool fromFile = false;
	};
	const TextureDescriptor DefaultForFilepath = {
		TextureFilter::LINEAR,               // filter
		MipmapSetting::LINEAR_MIPMAP_LINEAR, // settings = 
		TextureWrapping::CLAMP_TO_BORDER,    // wrapping =
		TextureWrapping::CLAMP_TO_BORDER,    // wrapping =
		0, 0, 0, true
	};

	//This class doesn't keep any internal data or paths
	//No more transitive dependencies
	class Texture
	{
	public:
		Texture() {}
		Texture(unsigned int external_id, const std::string path, TextureDescriptor = DefaultForFilepath);
		Texture(unsigned int external_id, const std::string path, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);
		Texture(unsigned int external_id, unsigned char* texture_data, int texture_width, int texture_height, GLenum Type, TextureFilter filter = TextureFilter::LINEAR, MipmapSetting settings = MipmapSetting::LINEAR_MIPMAP_LINEAR, TextureWrapping wrapping = TextureWrapping::CLAMP_TO_BORDER);

		void deleteTexture();
		void bind();
		inline unsigned int getID() const { return i_TextureId; }
		inline unsigned int getHash() const { return i_Hash; }
		void setSize(int width, int height);
		std::array<int, 2> getSize();
		void flushData(unsigned char* data); //assume doesn't change texture sizes
		void flushData(int width, int heigth, unsigned char* data);
	private:
		void genTextureFromFile(const std::string path);
		void genTextureFromData(unsigned char* data);
		unsigned int i_TextureId = -1;
		unsigned int i_Hash = -1;
	private:
		TextureDescriptor i_descriptor = {
			TextureFilter::LINEAR,               // filter
			MipmapSetting::LINEAR_MIPMAP_LINEAR, // settings = 
			TextureWrapping::CLAMP_TO_BORDER,    // wrapping_s =
			TextureWrapping::CLAMP_TO_BORDER,    // wrapping_t =
		};

	};

};



#endif

