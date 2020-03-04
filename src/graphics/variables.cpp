#include "variables.h"

namespace gph = graphics;

int gph::windowWidth = 1280;
int gph::windowHeight = 720;
const char* gph::windowName = "Pof Engine";
bool gph::quit = false;
bool gph::windowResized = false;
GLuint gph::vertexArrayID;
GLuint gph::vertexBuffer;

//just as a test, will remove later when adding proper graphics
float gph::triangleVertices[9] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

uint gph::triangleIndices[6] = {
	0,1,3,
	1,2,3
};