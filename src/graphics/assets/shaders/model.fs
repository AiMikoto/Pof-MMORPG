#version 460 core

const uint MAX_TEXTURES = 30;
const int multiply = 0;
const int add = 1;
const int substract = 2;
const int divide = 3;
const int smoothAdd = 4;
const int signedAdd = 5;

const int diffuseTexture = 0;
const int specularTexture = 1;
const int ambientTexture = 2;
const int emmisiveTexture = 3;
const int transparentColor = 4;

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

vec4 colorDiffuse, colorSpecular, colorAmbient, colorEmissive, colorTransparent;

void main() {
	for(int i = 0; i < material.texturesCount; i++) {
		switch(material.texturesType[i]) {
		case diffuseTexture:
			colorDiffuse = texture(material.textures[i], uvs);// * material.colorDiffuse;
			break;
		case specularTexture:
			colorSpecular = texture(material.textures[i], uvs);// * material.colorSpecular;
			break;
		}
	}

	fragmentColor = colorDiffuse + colorSpecular;


	//fragmentColor = vec4(uvs.x, uvs.y, 1, 1);
}