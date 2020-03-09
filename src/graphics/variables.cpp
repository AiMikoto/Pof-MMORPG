#include "variables.h"

namespace gph = graphics;

int gph::windowWidth = 1280;
int gph::windowHeight = 720;
const char* gph::windowName = "Pof Engine";
bool gph::quit = false;
bool gph::windowResized = false;
GLuint gph::vertexArrayID;
GLuint gph::vertexBufferID, gph::textureBufferID, gph::elementBufferID;
double gph::deltaTime;

//just as a test, will remove later when adding proper graphics
float gph::vertices[12] = {
	0.5f,  0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,
   -0.5f,  0.5f, 0.0f
};

float gph::uvs[8] = {
	1,1,  1,0,	0,0,  0,1
};

uint gph::indices[6] = {
	0,1,3,
	3,1,2
};