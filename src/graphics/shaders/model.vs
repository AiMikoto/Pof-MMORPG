#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 mvp;

out vec4 vertexColor;
out vec2 uvs;

void main() {
	uvs = textureCoordinates;
    gl_Position = mvp * vec4(position, 1.0);
    vertexColor = vec4(0, 0.3, 0.8, 1.0); // make a light blue triangle peepoHappy
	
}