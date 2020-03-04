#include "utils.h"

namespace gph = graphics;

std::vector<std::string> gph::charArrayToStringVector(const char* const input[], size_t arrayLength) {
	std::vector<std::string> output;
	for (size_t i = 0; i < arrayLength; i++) {
		output.push_back(input[i]);
	}
	return output;
}

std::vector<std::string> gph::split(const std::string &toSplit, char separator)
{
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = toSplit.find(separator, start)) != std::string::npos)
	{
		if (end != start)
			tokens.push_back(toSplit.substr(start, end - start));
		start = end + 1;
	}
	if (end != start)
		tokens.push_back(toSplit.substr(start));
	return tokens;
}