#pragma once
#include <vector>
#include <string>
#include <map>
#include "graphics_files.h"
#include <boost/math/constants/constants.hpp>

using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;
using llong = long long;
using ullong = unsigned long long;
using ulong = unsigned long;
using ldouble = long double;
const double pi = boost::math::constants::pi<double>();

namespace graphics {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...

	const uint requiredShadersPathLength = 2;
	constexpr const char* const requiredShadersPath[] = 
		{ "../src/graphics/shaders/model.vs", "../src/graphics/shaders/model.fs"};

	const auto totalCameraMovements = 6;

	namespace colors {
		const glm::vec4 red = glm::vec4(1, 0, 0, 1);
		const glm::vec4 green = glm::vec4(0, 1, 0, 1);
		const glm::vec4 blue = glm::vec4(0, 0, 1, 1);
		const glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		const glm::vec4 gray = glm::vec4(0.5f, 0.5f, 0.5f, 1);
	};

	namespace cam{
		enum cameraMovements {
			forward, backwards, up, down, left, right
		};
	}

	namespace objectTypes {
		enum types {
			object, mesh, camera, light, collider
		};
	};

	enum shaderTypes {
		modelShader
	};

	enum movementKeys {
		forward = GLFW_KEY_W,
		backwards = GLFW_KEY_S,
		left = GLFW_KEY_A,
		right = GLFW_KEY_D
	};
}
