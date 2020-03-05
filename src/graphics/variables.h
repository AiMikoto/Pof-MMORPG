#pragma once
#include <vector>
#include <string>
#include "graphics_files.h"
#include "constants.h"

namespace graphics {
	extern int windowWidth, windowHeight;
	extern bool windowResized, quit;
	extern const char* windowName;
	extern GLuint vertexArrayID;
	extern GLuint vertexBuffer;

	extern float triangleVertices[9];
	extern uint triangleIndices[6];
	extern double deltaTime;
}