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
}

std::vector<glm::vec4> HelperFunctions::calculateBezierCurve(const std::vector<glm::vec4>& controlPoints, float timeStep) {
	std::vector<glm::vec4> points;

	for (auto t = 0.0f; t < 1.0f; t += timeStep) {
		glm::vec4 point;
		float oneMinT = 1.0f - t;
		point = powf(oneMinT, 3) * controlPoints[0] + 3 * powf(oneMinT, 2) * t * controlPoints[1] + 3 * oneMinT * t * t * controlPoints[2] + powf(t, 3) * controlPoints[3];
		points.push_back(point);
	}

	return points;
}