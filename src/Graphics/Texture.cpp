#include "Texture.h"
#ifdef GLFW_PLATFORM_ENABLED
#error GLFW platform dependency introduced in Render
#endif

#include "../Util/Util.h"
#include <stb_image.h>

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{
	Texture::Texture(unsigned int external_id, const std::string path, TextureDescriptor descriptor)
	{
	}
	Texture::Texture(unsigned int hash, const std::string path,
						TextureFilter   filter,
						MipmapSetting   setting,
						TextureWrapping  wrapping) :
							i_Hash       (hash),
							i_descriptor { filter , setting, wrapping, wrapping, 0, -1, -1, true}

	{
		glGenTextures(1, &i_TextureId);
		genTextureFromFile(path);

	}

	Texture::Texture(
						unsigned int hash,
						unsigned char* texture_data,
						int texture_width,
						int texture_height,
						GLenum type,
						TextureFilter filter,
						MipmapSetting setting,
						TextureWrapping wrapping
					) :
						i_Hash(hash),
						i_descriptor{ filter , setting, wrapping, wrapping, type, texture_width, texture_height, false }
	{
		glGenTextures(1, &i_TextureId);
		genTextureFromData(texture_data);
	}

	void Texture::genTextureFromFile(const std::string path)
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_descriptor.wrapping_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_descriptor.wrapping_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_descriptor.setting);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_descriptor.filter);

		unsigned char* pTextureData;

		//TODO: be aware, this is not the correct way to guess the format
		//bool usePng = std::filesystem::path(i_path).extension() == ".png";

		stbi_set_flip_vertically_on_load(true);

		int nrChannels;
		pTextureData = stbi_load(path.c_str(), &i_descriptor.width, &i_descriptor.height, &nrChannels, 0); // Maybe move this process to Loader.h
		//Use scope_guard to free texture data whenever going out of scope
		util::scope_guard freeTextureData([pTextureData]() { if (pTextureData) stbi_image_free(pTextureData); });

		switch (nrChannels)
		{
		case 1: i_descriptor.type = GL_RED;  break;
		case 3: i_descriptor.type = GL_RGB;  break;
		case 4: i_descriptor.type = GL_RGBA; break;
		}


		if (!pTextureData)
		{
			messages::PrintMessage("Failed to load Texture at path: " + path, "Mesh.cpp/zap::Texture::Texture(...)", MessageTypes::error)
				<< "current working directory: " << std::filesystem::current_path() << std::endl;
			return;
		}

		//TODO: be aware of internal format and format difference, it can cause shader performance downfall
		glTexImage2D(GL_TEXTURE_2D, 0, i_descriptor.type, i_descriptor.width, i_descriptor.height, 0, i_descriptor.type, GL_UNSIGNED_BYTE, pTextureData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Texture::setSize(int width, int height)
	{
		i_descriptor.width = width; i_descriptor.height = height;
	}
	std::array<int, 2> Texture::getSize()
	{
		return std::array<int, 2>  { i_descriptor.width, i_descriptor.height };
	}
	void Texture::flushData(unsigned char* data)
	{
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, i_descriptor.type, i_descriptor.width, i_descriptor.height, 0, i_descriptor.type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::flushData(int width, int heigth, unsigned char* data)
	{
		bind();
		i_descriptor.width = width, i_descriptor.height = heigth;
		glTexImage2D(GL_TEXTURE_2D, 0, i_descriptor.type, i_descriptor.width, i_descriptor.height, 0, i_descriptor.type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::genTextureFromData(unsigned char* data)
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_descriptor.wrapping_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_descriptor.wrapping_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_descriptor.setting);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_descriptor.filter);

		glTexImage2D (GL_TEXTURE_2D, 0, i_descriptor.type, i_descriptor.width, i_descriptor.height, 0, i_descriptor.type, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::deleteTexture()
	{
		glDeleteTextures(1, &i_TextureId);
	}

	void Texture::bind() 
	{ 
		glBindTexture(GL_TEXTURE_2D, i_TextureId);
	}



}



