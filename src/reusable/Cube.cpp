#include <reusable/Cube.h>

Cube::Cube()
	: scale(1.0f), VAO(0), VBO(0), EBO(0), isSkybox(false)
{
	setupCube();
}

Cube::Cube(float scale)
	: scale(scale), VAO(0), VBO(0), EBO(0), isSkybox(false)
{
	setupCube();
}

Cube::Cube(bool isSkybox)
	: scale(1.0f), VAO(0), VBO(0), EBO(0), isSkybox(false)
{
	setupSkybox();
}

Cube::~Cube() 
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube::setScale(float scale)
{
	this->scale = scale;
	setupCubeVertices(vertices, scale);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cube::setupCube() {
	setupCubeVertices(vertices, scale);

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Texture Coordinates attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::setupSkybox() {
	setupCubeVertices(vertices, scale);

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 10,
		10, 11, 8,
		12, 13, 14,
		14, 15, 12,
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::setupCubeVertices(float* vertices, float scale)
{
	float halfSize = scale / 2.0f;

	float tempVertices[] = {
		// positions						  // normals	 // texture coords
		-halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 halfSize, -halfSize, -halfSize,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-halfSize,  halfSize, -halfSize,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		-halfSize, -halfSize,  halfSize,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 halfSize, -halfSize,  halfSize,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 halfSize,  halfSize,  halfSize,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-halfSize,  halfSize,  halfSize,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

		-halfSize,  halfSize,  halfSize, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-halfSize,  halfSize, -halfSize, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-halfSize, -halfSize, -halfSize, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-halfSize, -halfSize,  halfSize, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		 halfSize,  halfSize,  halfSize,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 halfSize,  halfSize, -halfSize,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 halfSize, -halfSize, -halfSize,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 halfSize, -halfSize,  halfSize,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		-halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		 halfSize, -halfSize, -halfSize,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		-halfSize, -halfSize,  halfSize,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		 halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-halfSize,  halfSize, -halfSize,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 halfSize,  halfSize,  halfSize,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	};

	memcpy(vertices, tempVertices, sizeof(tempVertices));
}

