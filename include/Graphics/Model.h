#pragma once

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <Graphics/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/Mesh.h"
#include "Graphics/Shader.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

class Model
{
	public:

		vector<TextureInfo> textures_loaded;
		vector<Mesh> meshes;
		string directory;
		bool gammaCorrection;

		// Constructor
		Model(std::string const& path, bool gamma = false);

		// Draw the model
		void Draw(Shader shader);

	private:

		void loadModel(string const& path);

		void processNode(aiNode* node, const aiScene* scene);

		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};