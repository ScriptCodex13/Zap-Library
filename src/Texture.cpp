#include "Mesh.h"

#include <stb_image.h>

/**************************************************************************************/

// extern includes

#include <filesystem>

/**************************************************************************************/

namespace zap
{

	Texture::Texture(unsigned int id, const std::string path,
						unsigned int     texture_index,
						TextureFilters   filter,
						MipmapSettings   settings,
						TextureWrapping  wrapping) :
		i_id             (id),
		i_path           (path),
		i_filter         (filter),
		i_settings       (settings),
		i_wrapping       (wrapping),
		i_texture_index  (texture_index)

	{
		glGenTextures(1, &i_texture);
	}

	void Texture::genTexture()
	{
		/*namespace fs = std::filesystem; // I wouldn't use namespaces here 
		using std::cout;
		using std::endl;*/

		//glBindTexture (GL_TEXTURE_2D, i_texture);
		
		bind();

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)i_wrapping);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)i_settings);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)i_filter);

		bool usePng = std::filesystem::path(i_path).extension() == ".png"; //TODO: so what? 

		stbi_set_flip_vertically_on_load(true);
		//a memory buffer never should be member of class, 
		//It is needed only inside this function.
		//it is never needed at any moments of program execution
		//so declare, create and destroy the buffer right away

		unsigned char* i_texturedata = stbi_load(i_path.c_str(), &i_width, &i_height, &i_nrChannels, 0); // Bessere Lösung nutze Filepath wie im Tutorial !
		if (!i_texturedata)
		{
			messages::PrintMessage("Failed to load Texture at path: " + i_path, "Mesh.cpp/zap::Texture::Texture(...)", MessageTypes::error)
				<< "current working directory: " << std::filesystem::current_path() << std::endl;
			return;
		}

		if (usePng)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i_width, i_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, i_texturedata);
		}
		else 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_width, i_height, 0, GL_RGB, GL_UNSIGNED_BYTE, i_texturedata);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		//since this point i_texturedata is no more needed. We do stbi_image_free here
		//no more memory managemet
		//No more destructor for Texture class
		stbi_image_free(i_texturedata);
	}
	void Texture::bind() 
	{ 
		glBindTexture(GL_TEXTURE_2D, i_texture); 
	}



}


