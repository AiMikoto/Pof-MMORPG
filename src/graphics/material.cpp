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

gph::Material::Material(uint shaderID) {
	this->shaderID = shaderID;
}

void gph::Material::contextSetup() {
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

boost::property_tree::ptree gph::Material::serialize() {}

void gph::Material::deserialize(boost::property_tree::ptree node) {}