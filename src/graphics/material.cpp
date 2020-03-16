#include "material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "graphics_files.h"
#include "lib/log.h"
#include "gpu.h"

namespace gph = graphics;

gph::Texture::Texture() {}

gph::Texture::Texture(std::string path) {
	this->path = path;
	load();
}

gph::Texture::Texture(std::string path, uint type) {
	this->path = path;
	this->type = type;
	load();
}

gph::Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void gph::Texture::load() {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, totalChannels;
	stbi_set_flip_vertically_on_load(true);
	uchar* data = stbi_load(path.c_str(), &width, &height, &totalChannels, 0);

	if (data) {
		GLenum format;
		switch (totalChannels) {
		case 1:
			format = GL_RED;
		case 3:
			format = GL_RGB;
		case 4:
			format = GL_RGBA;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		BOOST_LOG_TRIVIAL(trace) << "Failed to load texture at: " << path;
	}
	stbi_image_free(data);
	gpu->textures[this->id] = this;
}

gph::Material::Material() {}

gph::Material::~Material() {}

gph::Material::Material(uint textureID, uint shaderID, glm::vec4 color) {
	this->textureID = textureID;
	this->shaderID = shaderID;
	this->color = color;
}

void gph::Material::shaderSetup(float alpha) {
	gpu->shaders[shaderID]->setVec4("color", color);
	gpu->shaders[shaderID]->setFloat("matAlpha", alpha);
	if (gpu->textures.find(textureID) != gpu->textures.end()) {
		glActiveTexture(GL_TEXTURE0 + textureID);
		gpu->shaders[shaderID]->setInt("textureSampler", textureID);
		gpu->shaders[shaderID]->setBool("useTexture", true);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	gpu->shaders[shaderID]->setBool("useTexture", false);
}