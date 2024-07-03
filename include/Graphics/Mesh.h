#pragma once

#include <glad/glad.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Graphics/Camera.h"
#include "Graphics/Texture.h"

#include <vector>
#include <string>
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


// Mesh class
class Mesh {
public:

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	unsigned int VAO;

	// Constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

    // render the mesh
    void Draw(Shader& shader);

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
	void setupMesh();
};