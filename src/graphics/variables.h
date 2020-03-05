#pragma once
#include <vector>
#include <string>
#include "constants.h"

namespace graphics {
	extern int windowWidth, windowHeight;
	extern bool windowResized, quit;
	extern const char* windowName;
	extern GLuint vertexArrayID;
	extern GLuint vertexBuffer;
	extern GLuint elementBuffer;

	extern float cubeVertices[24];
	extern GLuint cubeIndices[36];
	extern double deltaTime;
}