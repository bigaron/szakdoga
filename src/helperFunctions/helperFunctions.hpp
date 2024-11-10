#ifndef HELPERFUNCTIONS_HELPERFUNCTIONS_HPP
#define HELPERFUNCTIONS_HELPERFUNCTIONS_HPP
#include <vector>
#include <string>

#include "glm/glm.hpp"

class HelperFunctions {
public:
	static std::vector<std::string> split(const std::string& line, const std::string& delim);
	static std::vector<glm::vec4> calculateBezierCurve(const std::vector<glm::vec4>& controlPoints, float timeStep);
	static std::vector<glm::vec4> calculateLine(const glm::vec4& startP, const glm::vec4& endP, float gran);
};
#endif // !HELPERFUNCTIONS_HELPERFUNCTIONS_HPP
