#pragma once
#include <vector>
#include <string>
#include "constants.h"

namespace graphics {
	extern int windowWidth, windowHeight;
	extern bool windowResized, quit;
	extern const char* windowName;
	extern GLuint vertexArrayID;
	extern GLuint vertexBufferID, textureBufferID, elementBufferID;

	extern float vertices[12];
	extern float uvs[8];
	extern uint indices[6];
	extern double deltaTime;
}