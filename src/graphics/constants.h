#pragma once
#include <vector>
#include <string>

using uint = unsigned int;
namespace graphics {
	//Too bad we're not using c++ 20, or we could just use vectors and strings in constexpr,
	//I need this ugly workaround, oh well...
	const uint requiredShadersPathLength = 4;
	constexpr const char* const requiredShadersPath[] = 
		{ "vertex_shader1.path", "fragment_shader1.path",
		  "vertex_shader2.path", "fragment_shader2.path"};
}