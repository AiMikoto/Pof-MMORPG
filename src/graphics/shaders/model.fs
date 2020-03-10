#version 460 core

in vec4 vertexColor;
in vec2 uvs;

uniform sampler2D diffuseSampler1;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(diffuseSampler1, uvs);
	//fragmentColor = vertexColor;
}