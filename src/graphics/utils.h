#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "constants.h"
#include "variables.h"

namespace graphics {
	std::vector<std::string> charArrayToStringVector(const char* const input[], size_t arrayLength);
	std::vector<std::string> split(const std::string &toSplit, char separator);
}