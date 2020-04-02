#pragma once
#include "glm/gtc/type_ptr.hpp"
#include <boost/math/constants/constants.hpp>

using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;
using llong = long long;
using ullong = unsigned long long;
using ulong = unsigned long;
using ldouble = long double;
const double pi = boost::math::constants::pi<double>();

namespace engine {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...
	
	const uint requiredShadersPathLength = 2;
	constexpr const char* const requiredShadersPath[] = 
		{ "../src/graphics/assets/shaders/model.vs", "../src/graphics/assets/shaders/model.fs"};

	namespace colors {
		const glm::vec4 red = glm::vec4(1, 0, 0, 1);
		const glm::vec4 green = glm::vec4(0, 1, 0, 1);
		const glm::vec4 blue = glm::vec4(0, 0, 1, 1);
		const glm::vec4 bgColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
		const glm::vec4 gray = glm::vec4(0.5f, 0.5f, 0.5f, 1);
		const glm::vec4 black = glm::vec4(0, 0, 0, 1);
	};

	enum shaderTypes {
		modelShader
	};

	const uint maxRenderedInstances = 512;

	enum componentTypes {
		camera, light, meshFilter, meshRenderer, physCollider, solidObject
	};
}
