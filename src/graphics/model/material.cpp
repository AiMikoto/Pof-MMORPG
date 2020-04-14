#include "graphics/model/material.h"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#include "lib/log.h"
#include "graphics/gpu.h"
#include "include/glad.h"
#include "include/glfw3.h"
#include <iostream>
#include <fstream>
#include "core/exceptions.h"

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

engine::Texture::Texture(boost::property_tree::ptree node) {
	path = node.get<std::string>("path", "");
	type = node.get<uint>("type", type);
	load();
}

engine::Texture::~Texture() {
	glDeleteTextures(1, &id);
}

boost::property_tree::ptree engine::Texture::serialize() {
	boost::property_tree::ptree node;
	node.add("path", path);
	node.add("type", type);
	return node;
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
	uchar* data = stbi_load(path.c_str(), &width, &height, &totalChannels, 0);
	if (data) {
		GLenum format;
		switch (totalChannels) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		BOOST_LOG_TRIVIAL(trace) << "Failed to load texture at: " << path;
	}
	stbi_image_free(data);
	gpu->textures[path] = this;
}

engine::Material::Material() {}

engine::Material::~Material() {}

engine::Material::Material(uint shaderType) {
	this->shaderType = shaderType;
}

engine::Material::Material(boost::property_tree::ptree node) {

}

void engine::Material::contextSetup() {
	Shader* shader = gpu->shaders[shaderType];
	shader->use();
	shader->setVec4("material.colorDiffuse", colorDiffuse);
	shader->setVec4("material.colorSpecular", colorSpecular);
	shader->setVec4("material.colorAmbient", colorAmbient);
	shader->setVec4("material.colorEmissive", colorEmissive);
	shader->setVec4("material.colorTransparent", colorTransparent);
	shader->setFloat("material.opacity", opacity);
	shader->setInt("material.blend", blend);
	shader->setInt("material.shading", shading);
	shader->setInt("material.texturesCount", (int)texturesPaths.size());
	shader->setInt("material.texturesStrengthCount", (int)texturesStrength.size());
	shader->setInt("material.texturesOPCount", (int)texturesOP.size());
	for (int i = 0; i < int(texturesPaths.size()); i++) {
		glActiveTexture(GL_TEXTURE0 + gpu->textures[texturesPaths[i]]->id);
		shader->setInt("material.textures[" + std::to_string(i) + "]", gpu->textures[texturesPaths[i]]->id);
		shader->setInt("material.texturesType[" + std::to_string(i) + "]", gpu->textures[texturesPaths[i]]->type);
		if(i < texturesStrength.size())
			shader->setFloat("material.texturesStrength[" + std::to_string(i) + "]", texturesStrength[i]);
		if(i < texturesOP.size())
			shader->setInt("material.texturesOP[" + std::to_string(i) + "]", texturesOP[i]);
		glBindTexture(GL_TEXTURE_2D, gpu->textures[texturesPaths[i]]->id);
	}
}

boost::property_tree::ptree engine::Material::serialize() {
	boost::property_tree::ptree node, texNode;
	return node;
}

void engine::Material::writeToFile() {
	/*std::ofstream fileStream(path, std::ios::out);
	if (fileStream.is_open()) {
		fileStream.close();
	}
	else throw std::system_error(errno, std::system_category(), "Failed to create/write to file at: " + path);*/
}

void engine::Material::load() {

}