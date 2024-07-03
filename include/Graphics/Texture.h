#pragma once

#include <glad/glad.h>
#include <Graphics/stb_image.h>
#include <string>

#include "Graphics/Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	std::string textureType;
	std::string path;
	GLuint unit;

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
	Texture();

	// Assign texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	// Bind the texture
	void Bind();

	// Unbind the texture
	void Unbind();

	// Delete the texture
	void Delete();

};
