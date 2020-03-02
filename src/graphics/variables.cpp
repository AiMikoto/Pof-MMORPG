#include "variables.h"

namespace gph = graphics;

int gph::windowWidth = 1280;
int gph::windowHeight = 720;
const char* gph::windowName = "Pof Engine";
bool gph::quit = false;
bool gph::windowResized = false;
std::vector<GLuint> gph::shaderProgramsIDs;