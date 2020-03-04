#pragma once
#include <vector>
#include <string>
#include <map>

using uint = unsigned int;
using ushort = unsigned short;
using uchar = unsigned char;
using llong = long long;

namespace graphics {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...

	constexpr auto MODEL_SHADER = "model";
	const uint requiredShadersPathLength = 2;
	constexpr const char* const requiredShadersPath[] = 
		{ "../src/graphics/shaders/model.vertexshader", "../src/graphics/shaders/model.fragmentshader"};
}