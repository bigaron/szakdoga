#ifndef MONTECARLO_MONTECARLO_HPP
#define MONTECARLO_MONTECARLO_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "vertexAttrib.hpp"
#include "../helperFunctions/helperFunctions.hpp"


class MonteCarlo {
	std::vector<VertexAttrib> boundingPoints;
public:


	void readInputFromFile(const char* filePath);
};

#endif // !MONTECARLO_HPP
