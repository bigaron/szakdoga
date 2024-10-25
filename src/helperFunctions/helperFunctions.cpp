#include "helperFunctions.hpp"

std::vector<std::string> HelperFunctions::split(const std::string& line, const std::string& delim) {
	std::vector<std::string> values;
	size_t idx = line.find(delim);
	std::string tmp = line;

	while(idx != std::string::npos) {
		std::string substr = tmp.substr(0ull, idx);
		if (substr == delim) return std::vector<std::string>();
		values.push_back(substr);
		tmp = tmp.substr(idx + 1ull);
		idx = tmp.find(delim);
	}

	values.push_back(tmp);

	return values;
	/*size_t idx = line.find(delim), prevIdx = 0ull;

	while (idx != std::string::npos) {
		std::string substr = line.substr(prevIdx, idx - prevIdx);
		if (substr == delim) return std::vector<std::string>();
		values.push_back(substr);
		prevIdx = idx;
		idx = line.find(delim, idx+1ull);
	}
	values.push_back(line.substr(prevIdx+1ull));

	return values;*/
}