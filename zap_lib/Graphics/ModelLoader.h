#pragma once 

#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Util/Message.h"

namespace zap
{
	struct TextureData
	{
		std::string type_name;
		std::string path;
	};

	inline std::vector<TextureData> loaded_textures;

	struct MeshData
	{
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<float> normals;

		bool has_normals = true;

		bool has_texture_coords = true;
		bool has_texture_tangents = true;
		bool has_texture_bitangents = true;

		std::vector<float> texture_coords;
		std::vector<float> texture_tangents;
		std::vector<float> texture_bitangents;

		std::vector<TextureData> diffuse_maps;
		std::vector<TextureData> specular_maps;
		std::vector<TextureData> normal_maps;
		std::vector<TextureData> height_maps;
	};

	struct ModelData
	{
		std::vector<MeshData> meshes;
	};

	ModelData LoadModel(std::string path);
}

#endif