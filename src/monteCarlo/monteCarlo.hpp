#ifndef MONTECARLO_MONTECARLO_HPP
#define MONTECARLO_MONTECARLO_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <exception>

#include "vertexAttrib.hpp"
#include "../helperFunctions/helperFunctions.hpp"
#include "../shader.hpp"

class MonteCarlo {
	std::vector<VertexAttrib> boundingPoints;
	Shader monteCarloShader;
public:
	MonteCarlo(){
		boundingPoints = std::vector<VertexAttrib>();
		monteCarloShader = Shader();
		monteCarloShader.GraphicsShader("shaders/monteCarlo.vert", "shaders/monteCarlo.frag");
		monteCarloShader.ComputeShader("shaders/monteCarlo.comp");
	}

	void readInputFromFile(const char* filePath);
	void printBoundaryPoints();
};

#endif // !MONTECARLO_HPP
