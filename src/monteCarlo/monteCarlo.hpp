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
#include "MonteCarloParams.hpp"

class MonteCarlo {
	std::vector<VertexAttrib> boundingPoints;
	Shader monteCarloShader;
	std::string pathPref;
	MonteCarloParameters params;

	int screenHeight, screenWidth;

	GLuint vao;
	GLuint boundarySSBO, paramSSBO;
	GLuint windowUBO;
public:
	MonteCarlo(std::string pathPref=""):pathPref(pathPref) {
		boundingPoints = std::vector<VertexAttrib>();
		monteCarloShader = Shader();
		params = MonteCarloParameters();
	}

	void setupShaders();
	void readInputFromFile(const char* filePath);
	void printBoundaryPoints();
	void setPathPrefix(std::string path);
	void draw();

	void setParams(const MonteCarloParameters& params);
	void setupMonteCarlo(const MonteCarloParameters& params, int height = 720, int width = 1280);
	void setScreenDim(int height, int width);
};

#endif // !MONTECARLO_HPP
