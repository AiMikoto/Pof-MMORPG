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

	extern float triangleVertices[9];
	extern GLuint triangleIndices[6];
	extern double deltaTime;
}