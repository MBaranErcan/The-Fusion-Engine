#pragma once

#include <glad/glad.h>
#include <glm/glm/glm.hpp>

class Cube
{
public:
	Cube();
	Cube(float scale);
	Cube(bool isSkybox);
	~Cube();

	void setScale(float scale);
	void Draw();

private:
	unsigned int VAO, VBO, EBO;
	bool isSkybox;
	float scale;
	float vertices[288]; // 36 vertices * 8 floats [x, y, z, nx, ny, nz, s, t] (Positions, Normals, Texture Coordinates)

	void setupCube();
	void setupSkybox();
	static void setupCubeVertices(float* vertices, float scale);
};
