#include "Texture.h"
#include "../Util/Util.h"
#include <stb_image.h>

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{

	Texture::Texture(unsigned int id, const std::string path,
						TextureFilters   filter,
						MipmapSettings   settings,
						TextureWrapping  wrapping) :
		i_id             (id),
		i_path           (path),
		i_filter         (filter),
		i_settings       (settings),
		i_wrapping       (wrapping),
		i_width          (-1),
		i_height         (-1)

	{
		glGenTextures(1, &i_texture);
	}

	Texture::Texture // For Developers
	(
		unsigned int id,
		unsigned char* texture_data,
		int texture_width,
		int texture_height,
		GLenum Type,
		TextureFilters filter,
		MipmapSettings settings,
		TextureWrapping wrapping
	) :
		i_id(id),
		i_TextureData(texture_data),
		i_width(texture_width),
		i_height(texture_height),
		i_Type(Type),
		i_filter(filter),
		i_settings(settings),
		i_wrapping(wrapping)
	{
		i_path = "";
		glGenTextures(1, &i_texture);
		i_load_data_manual = true;
	}

	Texture::~Texture()
	{
		//delete i_TextureData;
	}

	void Texture::genTexture()
	{
		bind();

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);
;
		unsigned char* pTextureData;

		bool usePng = std::filesystem::path(i_path).extension() == ".png";

		if (!i_load_data_manual)
		{
			stbi_set_flip_vertically_on_load(true);

			pTextureData = stbi_load(i_path.c_str(), &i_width, &i_height, &i_nrChannels, 0); // Maybe move this process to Loader.h
			//Use scope_guard to free texture data whenever going out of scope
			util::scope_guard freeTextureData([pTextureData]() { if (pTextureData) stbi_image_free(pTextureData); });


			if (!pTextureData)
			{
				messages::PrintMessage("Failed to load Texture at path: " + i_path, "Mesh.cpp/zap::Texture::Texture(...)", MessageTypes::error)
					<< "current working directory: " << std::filesystem::current_path() << std::endl;
				return;
			}
		}
		else
		{
			pTextureData = i_TextureData;
		}


		if (i_load_data_manual)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, i_Type, i_width, i_height, 0, i_Type, GL_UNSIGNED_BYTE, pTextureData);
		}
		else
		{
			if (usePng)
			{
				//TODO: be aware of internal format and format difference
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i_width, i_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTextureData);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, pTextureData);
			}
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		//We do not need to free the texture data here anymore.
		//Also we do not manage memory manually here anymore. It is done by scope_guard.
	}
	void Texture::bind() 
	{ 
		glBindTexture(GL_TEXTURE_2D, i_texture); 
	}



}



