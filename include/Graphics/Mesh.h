#pragma once

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Graphics/Shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4 // Max number of bones that can influence a vertex

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	// Bone IDs which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// weigths of the bones
	float m_Weights[MAX_BONE_INFLUENCE];
};

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

// Convert string to TextureType
TextureType stringToTextureType(const std::string& typeName);

// TextureInfo struct to store texture information
struct TextureInfo {
	unsigned int id;
	TextureType type;
    string path;

	// Default constructor
	TextureInfo() : id(0), type(TextureType::DIFFUSE), path("") {}

	// Constructor
	TextureInfo(unsigned int id, TextureType type, string path) {
		this->id = id;
		this->type = type;
		this->path = path;
	}

};

// Mesh class
class Mesh {
public:

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<TextureInfo> textures;
	unsigned int VAO;

	// Constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<TextureInfo> textures);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
	void setupMesh();
};