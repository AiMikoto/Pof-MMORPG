#version 460 core

const uint MAX_TEXTURES = 30;
const int multiply = 0;
const int add = 1;
const int substract = 2;
const int divide = 3;
const int smoothAdd = 4;
const int signedAdd = 5;

const int diffuseTexture = 1;

in vec4 vertexColor;
in vec2 uvs;

struct Material {
	vec4 colorDiffuse, colorSpecular, colorAmbient, colorEmissive, colorTransparent;
	int blend, shading, texturesCount, texturesStrengthCount, texturesOPCount;
	float opacity, shininess, shininessStrength;
	sampler2D[MAX_TEXTURES] textures;
	int[MAX_TEXTURES] texturesType, texturesOP;
	float[MAX_TEXTURES] texturesStrength;
};

uniform Material material;

out vec4 fragmentColor;

vec4 determineTextureColor(int i) {
	vec4 color = texture(material.textures[i], uvs);
	return (i < material.texturesStrengthCount) ? color * material.texturesStrength[i]: color;
}

vec4 applyTexture(int i, vec4 color) {
	switch(material.texturesOP[i]) {
	case multiply:
		color *= determineTextureColor(i);
		break;
	case add:
		color += determineTextureColor(i);
		break;
	case substract:
		color -= determineTextureColor(i);
		break;
	case smoothAdd:
		vec4 t1 = color;
		vec4 t2 = determineTextureColor(i);
		color = t1 + t2 - t1*t2;
	case signedAdd:
		color += determineTextureColor(i) - vec4(0.5, 0.5, 0.5, 0.5);
	}
	return color;
}

vec4 applyTextureType(int i, vec4 color) {
	switch(material.texturesType[i]) {
	case diffuseTexture:
		color = applyTexture(i, color);
		break;
	}
	return color;
}

void main() {
	vec4 colorDiffuse, colorSpecular, colorAmbient, colorEmissive, colorTransparent;
	for(int i = 0; i < material.texturesCount; i++) {
		applyTextureType(i, colorDiffuse);
	}
	fragmentColor = colorDiffuse;
}