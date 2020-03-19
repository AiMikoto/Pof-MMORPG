#include "utils.h"
#include <sstream>

std::vector<std::string> engine::charArrayToStringVector(const char* const input[], size_t arrayLength) {
	std::vector<std::string> output;
	for (size_t i = 0; i < arrayLength; i++) {
		output.push_back(input[i]);
	}
	return output;
}

std::vector<std::string> engine::split(const std::string &toSplit, char separator)
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

boost::property_tree::ptree engine::dvec2serializer(glm::dvec2 in) {
	boost::property_tree::ptree node;
	node.add("x", in.x);
	node.add("y", in.y);
	return node;
}

boost::property_tree::ptree engine::dvec3serializer(glm::dvec3 in) {
	boost::property_tree::ptree node;
	node.add("x", in.x);
	node.add("y", in.y);
	node.add("z", in.y);
	return node;
}

boost::property_tree::ptree engine::dvec4serializer(glm::dvec4 in) {
	boost::property_tree::ptree node;
	node.add("w", in.w);
	node.add("x", in.x);
	node.add("y", in.y);
	node.add("z", in.z);
	return node;
}

double engine::highestCommonDenominator(double a, double b, double eps) {
	a = std::fabs(a);
	b = std::fabs(b);
	if (a < b)
		return highestCommonDenominator(b, a);
	if (std::fabs(b) < eps)
		return a;
	else
		return highestCommonDenominator(b, a - floor(a / b) * b);
}