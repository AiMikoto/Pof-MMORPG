#version 460 core

in vec4 vertexColor;
in vec2 uvs;

uniform sampler2D textureSampler;
uniform vec4 color;
uniform float alpha;
uniform bool useTexture;

out vec4 fragmentColor;

void main() {
	vec4 alphaCorrection = vec4(1,1,1,alpha);
	if(useTexture) {
		fragmentColor = texture(textureSampler, uvs) * alphaCorrection;
	}
	else {
		fragmentColor = color * alphaCorrection;
	}
}