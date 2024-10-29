#include "monteCarlo.hpp"

void MonteCarlo::setupShaders() {
	monteCarloShader.GraphicsShader(std::string(pathPref).append("shaders/monteCarlo.vert").c_str(), std::string(pathPref).append("shaders/monteCarlo.frag").c_str());
	monteCarloShader.ComputeShader(std::string(pathPref).append("shaders/monteCarlo.comp").c_str());
}

void MonteCarlo::readInputFromFile(const char* filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "The file at the given filepath: [" << filePath << "] could not be opened!" << std::endl << "Returning..." << std::endl;
		return;
	}

	std::string line;
	size_t posDim, colDim;
	std::string valueSep = ";", typeSep = "," , inHouseSep = " ";
	
	std::getline(file, line);
	std::vector<std::string> splitArr = HelperFunctions::split(line, " ");
	if (splitArr.size() != 2) 
		throw std::invalid_argument("The file format specified is not correct. Please consult the README file for the proper format. ");
	

	posDim = std::stoull(splitArr[0]);
	colDim = std::stoull(splitArr[1]);

	std::istream& ret = std::getline(file, line);
	//We check if the input was formatted correctly
	if (ret.fail()) 
		throw std::invalid_argument("The file format specified is not correct. Please consult the README file for the proper format. ");

	if (line != "") {
		splitArr = HelperFunctions::split(line, " ");
		if (splitArr.size() != 2) 
			throw std::invalid_argument("The file format specified is not correct. Please consult the README file for the proper format. ");

		valueSep = splitArr[0][0];
		typeSep = splitArr[1][0];
		if (std::getline(file, line).fail()) 
			throw std::invalid_argument("The file format specified is not correct. Please consult the README file for the proper format. ");

	}

	std::vector<std::string> posVals, colVals;
	size_t lineNum = 1ull;

	glm::vec4 defaultVec4 = glm::vec4(0,0,0,1);

	//main loop of reading in values
	while (std::getline(file, line)) {
		splitArr = HelperFunctions::split(line, typeSep);
		if (splitArr.size() != 2) 
			throw std::invalid_argument("The file format specified is not correct. Please consult the README file for the proper format. ");

		posVals = HelperFunctions::split(splitArr[0], valueSep);
		if (posVals.size() != posDim) 
			throw std::invalid_argument("The number of position values in line " + std::to_string(lineNum)+ " doesnt match the specified position dimension " + std::to_string(posDim));

		size_t i = 0ull;
		glm::vec4 pos = defaultVec4;
		for (const std::string& elem : posVals) {
			pos[i] = std::stof(elem);
			i++;
		}

		colVals = HelperFunctions::split(splitArr[1], valueSep);
		if(colVals.size() != colDim)
			throw std::invalid_argument("The number of colour values in line " + std::to_string(lineNum) + " doesnt match the specified colour dimension " + std::to_string(colDim));

		i = 0ull;
		glm::vec4 col = defaultVec4;
		for (const std::string& elem : colVals) {
			col[i] = std::stof(elem);
			i++;
		}

		this->boundingPoints.emplace_back(pos, col);
		lineNum++;
	}
}

void MonteCarlo::printBoundaryPoints() {
	for (const VertexAttrib& bound : this->boundingPoints) {
		std::cout << bound.pos.x << " " << bound.pos.y << " " << bound.pos.z << " " << bound.pos.w << " || " << bound.col.r << " " << bound.col.g << " " << bound.col.b << " " << bound.col.a << std::endl;
	}
}

void MonteCarlo::setPathPrefix(std::string pathPref){
	this->pathPref = pathPref;
}

void MonteCarlo::draw() {



}

void MonteCarlo::setParams(const MonteCarloParameters& params) {
	this->params = params;
}

void MonteCarlo::setupMonteCarlo(const MonteCarloParameters& params, int height=720, int width=1280){
	this->setParams(params);
	this->setScreenDim(height, width);
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glCreateBuffers(1, &this->boundarySSBO);
	glCreateBuffers(1, &this->paramSSBO);
	glCreateBuffers(1, &this->windowUBO);

	glm::vec4 resVec(this->screenWidth, this->screenHeight, 0., 0.);
	glNamedBufferStorage(this->windowUBO, sizeof(glm::vec4), static_cast<const void*>(& resVec), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->windowUBO);

	glNamedBufferStorage(this->boundarySSBO, sizeof(VertexAttrib) * this->boundingPoints.size(), 
		static_cast<const void*>(this->boundingPoints.data()), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->boundarySSBO);

	glNamedBufferStorage(this->paramSSBO, sizeof(MonteCarloParameters), static_cast<const void*>(&this->params), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->paramSSBO);
}

void MonteCarlo::setScreenDim(int height, int width) {
	this->screenHeight = height;
	this->screenWidth = width;
}
