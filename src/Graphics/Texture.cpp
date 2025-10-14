#include "Texture.h"
#include "../Util/Util.h"
#include <stb_image.h>

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{

	Texture::Texture(unsigned int external_id, const std::string path,
						TextureFilters   filter,
						MipmapSettings   settings,
						TextureWrapping  wrapping) :
							i_ExternalId(external_id),
							i_path           (path),
							i_filter         (filter),
							i_settings       (settings),
							i_wrapping       (wrapping),
							i_width          (-1),
							i_height         (-1)

	{
		glGenTextures(1, &i_TextureId);
	}

	Texture::Texture // For Developers
					(
						unsigned int external_id,
						unsigned char* texture_data,
						int texture_width,
						int texture_height,
						GLenum Type,
						TextureFilters filter,
						MipmapSettings settings,
						TextureWrapping wrapping
					) :
						i_ExternalId(external_id),
						i_TextureData(texture_data),
						i_width(texture_width),
						i_height(texture_height),
						i_Type(Type),
						i_filter(filter),
						i_settings(settings),
						i_wrapping(wrapping)
	{
		glGenTextures(1, &i_TextureId);
	}

	void Texture::genTextureFromFile()
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);

		unsigned char* pTextureData;

		//TODO: be aware, this is not the correct way to guess the format
		//bool usePng = std::filesystem::path(i_path).extension() == ".png";

		stbi_set_flip_vertically_on_load(true);

		pTextureData = stbi_load(i_path.c_str(), &i_width, &i_height, &i_nrChannels, 0); // Maybe move this process to Loader.h
		//Use scope_guard to free texture data whenever going out of scope
		util::scope_guard freeTextureData([pTextureData]() { if (pTextureData) stbi_image_free(pTextureData); });
		GLenum format = GL_RGB; //preguessed, nothing to do
		switch (i_nrChannels)
		{
		case 1: format = GL_RED;  break;
		case 3: format = GL_RGB;  break;
		case 4: format = GL_RGBA; break;
		}


		if (!pTextureData)
		{
			messages::PrintMessage("Failed to load Texture at path: " + i_path, "Mesh.cpp/zap::Texture::Texture(...)", MessageTypes::error)
				<< "current working directory: " << std::filesystem::current_path() << std::endl;
			return;
		}

		assert(i_TextureData == nullptr && "load from file is one time set");
		//TODO: be aware of internal format and format difference, it can cause shader performance downfall
		glTexImage2D(GL_TEXTURE_2D, 0, format, i_width, i_height, 0, format, GL_UNSIGNED_BYTE, pTextureData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::setData(unsigned char* textureData)
	{
		i_TextureData = textureData;
	}
	void Texture::setSize(int width, int height)
	{
		i_width = width; i_height = height;
	}
	std::array<int, 2> Texture::getSize()
	{
		return std::array<int, 2>  { i_width, i_height };
	}
	void Texture::flushData()
	{
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, i_width, i_height, 0, i_Type, GL_UNSIGNED_BYTE, i_TextureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::genTextureFromData()
	{
		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);

		glTexImage2D(GL_TEXTURE_2D, 0, i_Type, i_width, i_height, 0, i_Type, GL_UNSIGNED_BYTE, i_TextureData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void Texture::deleteTexture()
	{
		glDeleteTextures(1, &i_TextureId);
	}
	void Texture::genTexture()
	{
		if (!i_path.empty())
		{
			genTextureFromFile();
		} 
		else
		{
			genTextureFromData();
		}
	}
	void Texture::bind() 
	{ 
		glBindTexture(GL_TEXTURE_2D, i_TextureId);
	}



}



