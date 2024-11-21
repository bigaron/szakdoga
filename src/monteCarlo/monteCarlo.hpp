#ifndef MONTECARLO_MONTECARLO_HPP
#define MONTECARLO_MONTECARLO_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>

#include "vertexAttrib.hpp"
#include "../helperFunctions/helperFunctions.hpp"
#include "../shader.hpp"
#include "MonteCarloParams.hpp"
#include "hostBVH.hpp"
#include "BVH.hpp"

class MonteCarlo {
	std::vector<VertexAttrib> boundingPoints, textureCorners;
	Shader monteCarloShader, bvhDebugShader, bvhDebugBoundingsShader;
	std::string pathPref;
	
	MonteCarloParameters params;
	AlgorithmOpts opts;

	int screenHeight = 0, screenWidth = 0;

	GLuint vao = 0u;
	GLuint boundarySSBO = 0u, paramSSBO = 0u, bvhSSBO = 0u, indexSSBO = 0u, algoSSBO = 0u, textureCornerSSBO = 0u;
	GLuint windowUBO = 0u;
	GLuint textureLoc = 0u, monteCarloTexture = 0u;
	GLuint vbo = 0u;

	BVH bvh ;
	std::vector<hostPointIndex> indices;
	std::vector<hostBVH> bvhToGPU;
	std::vector<glm::vec4> pixels;
	bool isDebugMode = false, isPaused = false;
	void printDistanceBetweenChildren(const Node& root);
public:
	MonteCarlo(std::string pathPref=""):pathPref(pathPref) {
		boundingPoints = std::vector<VertexAttrib>();
		monteCarloShader = Shader();
		bvhDebugShader = Shader();
		bvhDebugBoundingsShader = Shader();
		params = MonteCarloParameters();
		opts = AlgorithmOpts();
	}

	void setupShaders();
	void cpySSBOStoGPU();
	void readInputFromFile(const char* filePath);
	void printBoundaryPoints();
	void setPathPrefix(std::string path);
	void draw();

	void drawBVH();
	void setDebugMode(bool flag) { isDebugMode = flag; }
	void setPaused(bool flag) { isPaused = flag; }
	void getValueAtMouse(int x, int y);

	void generateBVH();

	void reset();

	void setParams(const MonteCarloParameters& params);
	void setupMonteCarlo(const MonteCarloParameters& params, int height = 720, int width = 1280);
	void setScreenDim(int height, int width);
};

#endif // !MONTECARLO_HPP
