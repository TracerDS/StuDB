#pragma once

#include <string>
#include <vector>

constexpr std::vector<std::string> splitString(std::string str, std::string delimiter) noexcept {
	std::vector<std::string> result;
	std::size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		if(!token.empty())
			result.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if (!str.empty())
		result.push_back(str);

	return result;
}

static size_t nearestMultipleOf(size_t num, size_t multiple) noexcept {
	return ((num + multiple - 1) / multiple) * multiple;
}