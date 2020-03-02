#pragma once
#include <vector>
#include <string>

using uint = unsigned int;
namespace graphics {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...
	const uint requiredShadersPathLength = 2;
	constexpr const char* const requiredShadersPath[] = 
		{ "../src/graphics/shaders/model.vertexshader", "../src/graphics/shaders/model.fragmentshader"};
}