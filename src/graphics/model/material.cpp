#include "graphics/model/material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/log.h"
#include "graphics/gpu.h"
#include "include/glad.h"
#include "include/glfw3.h"
#include <iostream>

engine::Texture::Texture() {}

engine::Texture::Texture(std::string path) {
	this->path = path;
	load();
}

engine::Texture::Texture(std::string path, uint type) {
	this->path = path;
	this->type = type;
	load();
}

engine::Texture::~Texture() {
	glDeleteTextures(1, &id);
}

void engine::Texture::load() {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int width, height, totalChannels;
	stbi_set_flip_vertically_on_load(true);
	uchar* data = stbi_load(path.c_str(), &width, &height, &totalChannels, 0);
	if (data) {
		GLenum internalFormat, format;
		switch (totalChannels) {
		case 1:
			internalFormat = GL_RED;
			format = GL_RED;
		case 3:
			internalFormat = GL_RGB8;
			format = GL_RGB8;
		case 4:
			internalFormat = GL_RGBA8;
			format = GL_RGBA8;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		BOOST_LOG_TRIVIAL(trace) << "Failed to load texture at: " << path;
	}
	stbi_image_free(data);
	gpu->textures[this->id] = this;
}

engine::Material::Material() {}

engine::Material::~Material() {}

engine::Material::Material(uint shaderID) {
	this->shaderID = shaderID;
}

void engine::Material::contextSetup() {
	Shader* shader = gpu->shaders[shaderID];
	shader->use();
	shader->setVec4("material.colorDiffuse", colorDiffuse);
	shader->setVec4("material.colorSpecular", colorSpecular);
	shader->setVec4("material.colorAmbient", colorAmbient);
	shader->setVec4("material.colorEmissive", colorEmissive);
	shader->setVec4("material.colorTransparent", colorTransparent);
	shader->setFloat("material.opacity", opacity);
	shader->setInt("material.blend", blend);
	shader->setInt("material.shading", shading);
	shader->setInt("material.texturesCount", (int)texturesIDs.size());
	shader->setInt("material.texturesStrengthCount", (int)texturesStrength.size());
	shader->setInt("material.texturesOPCount", (int)texturesOP.size());
	for (int i = 0; i < texturesIDs.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + texturesIDs[i]);
		shader->setInt("material.textures[" + std::to_string(i) + "]", texturesIDs[i]);
		shader->setInt("material.texturesType[" + std::to_string(i) + "]", gpu->textures[texturesIDs[i]]->type);
		if(i < texturesStrength.size())
			shader->setFloat("material.texturesStrength[" + std::to_string(i) + "]", texturesStrength[i]);
		if(i < texturesOP.size())
			shader->setInt("material.texturesOP[" + std::to_string(i) + "]", texturesOP[i]);
		glBindTexture(GL_TEXTURE_2D, texturesIDs[i]);
	}
}

boost::property_tree::ptree engine::Material::serialize() {}

void engine::Material::deserialize(boost::property_tree::ptree node) {}