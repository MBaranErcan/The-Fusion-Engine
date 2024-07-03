#include "Graphics/Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	: type(texType), path(image) {

	int widthImg, heightImg, numChImg;
	stbi_set_flip_vertically_on_load(true); // Flip the image vertically
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numChImg, 0);

	// Generate texture object
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Scaling options
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Wrapping options
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// Image allocation
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	// Free resources
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

Texture::Texture()
	: ID(0), type(GL_TEXTURE_2D), path(""), unit(0) {}


// Assign texture unit to a texture
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.use();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
		glDeleteTextures(1, &ID);
}

