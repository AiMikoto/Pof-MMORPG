#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include "glm/gtc/type_ptr.hpp"
#include <map>

namespace engine {
	std::vector<std::string> charArrayToStringVector(const char* const input[], size_t arrayLength);
	std::vector<std::string> split(const std::string &toSplit, char separator);
	template <typename T> std::string vectorToString(std::vector<T> in, char separator) {
		if (in.size() == 0) return "";
		std::string res;
		for (size_t i = 0; i < in.size() - 1; i++) {
			res += std::to_string(in[i]) + separator;
		}
		res += std::to_string(in[in.size() - 1]);
		return res;
	}
	template <typename T> std::vector<T> stringToVector(std::string in, char separator) {
		std::vector<T> res;
		std::vector<std::string> stringVector = split(in, separator);
		for (auto str : stringVector) {
			std::istringstream ss(str);
			T value;
			ss >> value;
			res.push_back(value);
		}
		return res;
	}
	boost::property_tree::ptree dvec2serializer(glm::dvec2);
	boost::property_tree::ptree dvec3serializer(glm::dvec3);
	boost::property_tree::ptree dvec4serializer(glm::dvec4);
	double highestCommonDenominator(double a, double b, double eps = 0.0001);
	//only works for maps that use numbers as a key
	template <typename T, typename S> T getFirstAvailableMapIndex(std::map<T, S> toCheck) {
		if (toCheck.size() == 0) {
			return (T(-1) < T(0)) ? T(0) : T(1);
		}
		return T(toCheck.rbegin()->first + 1);
	}
}