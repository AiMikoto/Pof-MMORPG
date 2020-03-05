#include "variables.h"

namespace gph = graphics;

int gph::windowWidth = 1280;
int gph::windowHeight = 720;
const char* gph::windowName = "Pof Engine";
bool gph::quit = false;
bool gph::windowResized = false;
GLuint gph::vertexArrayID;
GLuint gph::vertexBuffer;
GLuint gph::elementBuffer;
double gph::deltaTime;

//just as a test, will remove later when adding proper graphics
float gph::cubeVertices[24] = {
	-0.5f,  0.5f,  0.5f, //top left front
	 0.5f,  0.5f,  0.5f, //top right front
	-0.5f, -0.5f,  0.5f, //bottom left front
	 0.5f, -0.5f,  0.5f //bottom right front
	-0.5f,  0.5f, -0.5f, //top left back
	 0.5f,  0.5f, -0.5f, //top right back
	-0.5f, -0.5f, -0.5f, //bottom left back
	 0.5f, -0.5f, -0.5f //bottom right back
};

GLuint gph::cubeIndices[36] = {
	1,2,3,2,4,3, //front
	5,6,1,6,2,1, //top
	2,6,4,6,8,4, //right
	5,7,6,6,7,8, //back
	7,3,8,8,3,4, //bottom
	1,3,5,5,3,7 //left
};