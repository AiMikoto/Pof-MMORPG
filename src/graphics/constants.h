#pragma once
#include <vector>
#include <string>
#include <map>
#include "graphics_files.h"
#include <cmath>

using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;
using llong = long long;
const double pi = acos(-1);

namespace graphics {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...

	constexpr auto MODEL_SHADER = "model";
	const uint requiredShadersPathLength = 2;
	constexpr const char* const requiredShadersPath[] = 
		{ "../src/graphics/shaders/model.vertexshader", "../src/graphics/shaders/model.fragmentshader"};

	const auto totalCameraMovements = 6;

	namespace cam{
		enum cameraMovements {
			forward, backwards, up, down, left, right
		};
	}	

	enum movementKeys {
		forward = GLFW_KEY_W,
		backwards = GLFW_KEY_S,
		left = GLFW_KEY_A,
		right = GLFW_KEY_D
	};
}
