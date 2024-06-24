#pragma once

#include <glad/glad.h>
#include <Graphics/stb_image.h>

#include "Graphics/Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum textType, GLenum slot, GLenum format, GLenum pixelType);

	// Assign texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	// Bind the texture
	void Bind();

	// Unbind the texture
	void Unbind();

	// Delete the texture
	void Delete();

};
