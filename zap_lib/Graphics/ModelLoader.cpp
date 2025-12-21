#include "ModelLoader.h"

// TODO: This is only a prototype. We can find improvemets here.

namespace zap
{
	// Model loading func

	std::vector<TextureData> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string type_name)
	{
		std::vector<TextureData> textures;

		for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
		{
			aiString string;
			material->GetTexture(type, i, &string);

			bool skip_load = false;

			for (unsigned int s = 0; s < loaded_textures.size(); s++) // Checks if the texture is already loaded and uses the loaded one instead
			{
				if(loaded_textures[s].type_name == string.C_Str())
				{
					textures.push_back(loaded_textures[s]);
					skip_load = true;
					break;
				}
			}

			if(!skip_load)
			{
				TextureData data_config;
				data_config.path = string.C_Str();
				textures.push_back(data_config);
				loaded_textures.push_back(data_config);
			}
		}

		return textures;
	}

	MeshData ProcessMeshData(aiMesh* mesh, const aiScene* scene)
	{
		MeshData resultdata;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			resultdata.vertices.push_back(mesh->mVertices[i].x);
			resultdata.vertices.push_back(mesh->mVertices[i].y);
			resultdata.vertices.push_back(mesh->mVertices[i].z);

			if(mesh->HasNormals())
			{
				resultdata.has_normals = true;

				resultdata.normals.push_back(mesh->mNormals[i].x);
				resultdata.normals.push_back(mesh->mNormals[i].y);
				resultdata.normals.push_back(mesh->mNormals[i].z);
			}
			else
			{
				resultdata.has_normals = false;
			}

			if(mesh->mTextureCoords[0])
			{
				resultdata.has_texture_coords = true;

				resultdata.texture_coords.push_back(mesh->mTextureCoords[0][i].x);
				resultdata.texture_coords.push_back(mesh->mTextureCoords[0][i].y);

				resultdata.texture_tangents.push_back(mesh->mTangents[i].x);
				resultdata.texture_tangents.push_back(mesh->mTangents[i].y);
				resultdata.texture_tangents.push_back(mesh->mTangents[i].z);

				resultdata.texture_bitangents.push_back(mesh->mBitangents[i].x);
				resultdata.texture_bitangents.push_back(mesh->mBitangents[i].y);
				resultdata.texture_bitangents.push_back(mesh->mBitangents[i].z);
			}
			else 
			{
				resultdata.has_texture_coords = false;
				
				/*resultdata.texture_coords.push_back(0.0f);
				resultdata.texture_coords.push_back(0.0f);*/

				// Extend and use if really needed
			}
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for(unsigned int s = 0; s < face.mNumIndices; s++)
			{
				resultdata.indices.push_back(face.mIndices[s]);
			}
		}


		//	TODO: Add to this part more material properties later on

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		resultdata.diffuse_maps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse_t"); // The string is for identifying the texture type
		resultdata.specular_maps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "specular_t");
		resultdata.normal_maps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "noraml_t");
		resultdata.height_maps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "height_t");


		//

		return resultdata;
	}

	void ProcessNode(aiNode* node, const aiScene* scene, ModelData& resultdata)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			resultdata.meshes.push_back(ProcessMeshData(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, resultdata);
		}
	}

	//

	ModelData LoadModel(std::string path)
	{
		ModelData resultdata;


		// Load Scene

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile
		(
			path,
			aiProcess_Triangulate |
			aiProcess_GenNormals |
			aiProcess_FlipUVs |
			aiProcess_JoinIdenticalVertices
		);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			messages::PrintMessage("Failed to load model at path " + path + ": " + importer.GetErrorString(), "ModelData& zap::Mesh::LoadModel /Mesh.cpp", MessageTypes::error);
		}

		std::string directory = directory = path.substr(0, path.find_last_of('/'));

		//


		// Process Assimp's root node

		ProcessNode(scene->mRootNode, scene, resultdata);

		return resultdata;
	}
}