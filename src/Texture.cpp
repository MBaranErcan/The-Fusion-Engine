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

// Loader with vector containing the the paths to the cubemap faces
unsigned int Texture::loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(false); // Temporarily disable vertical flipping
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &numChannels, 0);
		if (data) {
			GLenum format = GL_RGB;
			if (numChannels == 4) {
				format = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
		}
	}
	stbi_set_flip_vertically_on_load(true); // Re-enable vertical flipping

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// Loader with path to the folder containing the cubemap faces
unsigned int Texture::loadCubemap(std::string path, std::string pictureType) {
	std::vector<std::string> faces = {
		"right.",
		"left.",
		"top.",
		"bottom.",
		"front.",
		"back."
	};

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(false); // Temporarily disable vertical flipping
	for (unsigned int i = 0; i < 6; i++) {
		std::string fullPath = path + (path.back() == '/' ? "" : "/") + faces[i] + pictureType;
		unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &numChannels, 0);
		if (data) {
			GLenum format = GL_RGB;
			if (numChannels == 4) {
				format = GL_RGBA;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << fullPath << std::endl;
		}
	}
	stbi_set_flip_vertically_on_load(true); // Re-enable vertical flipping

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
