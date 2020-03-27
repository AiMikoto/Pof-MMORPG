#version 460 core

//const uint maxRenderedInstances = 250;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in mat4 model;

//uniform mat4 models[maxRenderedInstances];
uniform mat4 vp;

out vec2 uvs;

void main() {
	uvs = textureCoordinates;
    //gl_Position = vp * models[gl_InstanceID] * vec4(position, 1.0);
	gl_Position = vp * model * vec4(position, 1.0);
}