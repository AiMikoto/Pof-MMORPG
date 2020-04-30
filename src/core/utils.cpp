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

glm::dvec3 engine::computeSize(std::vector<glm::dvec3> verticesPositions) {
	glm::dvec3 min = verticesPositions[0];
	glm::dvec3 max = verticesPositions[0];
	for (size_t i = 1; i < verticesPositions.size(); i++) {
		min.x = glm::min(min.x, double(verticesPositions[i].x));
		min.y = glm::min(min.y, double(verticesPositions[i].y));
		min.z = glm::min(min.z, double(verticesPositions[i].z));
		max.x = glm::max(max.x, double(verticesPositions[i].x));
		max.y = glm::max(max.y, double(verticesPositions[i].y));
		max.z = glm::max(max.z, double(verticesPositions[i].z));
	}
	return max - min;
}