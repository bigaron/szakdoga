#include "monteCarlo.hpp"

void MonteCarlo::setupShaders() {
	bvhDebugShader.GraphicsShader(std::string(pathPref).append("shaders/bvhDebug.vert").c_str(), std::string(pathPref).append("shaders/bvhDebug.frag").c_str());
	bvhDebugBoundingsShader.GraphicsShader(std::string(pathPref).append("shaders/boundingPoints.vert").c_str(), std::string(pathPref).append("shaders/boundingPoints.frag").c_str());
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
	
	this->params.vertexN = this->boundingPoints.size();
}

void MonteCarlo::printBoundaryPoints() {
	for (const VertexAttrib& bound : this->boundingPoints) {
		std::cout << bound.pos.x << " " << bound.pos.y << " " << bound.pos.z << " " << bound.pos.w << " || " << bound.col.r << " " << bound.col.g << " " << bound.col.b << " " << bound.col.a << std::endl;
	}
}

void MonteCarlo::setPathPrefix(std::string pathPref){
	this->pathPref = pathPref;
}

void MonteCarlo::drawBVHBox() {
	GLuint bvhVBO;
	glGenBuffers(1, &bvhVBO);
	glUseProgram(this->bvhDebugShader.graphicsID);

	unsigned int loc = bvhDebugShader.getUniformLocation("color", bvhDebugShader.graphicsID);
	glUniform4f(loc, 0.0f, 1.0f, 0.0f, 1.0f);
	glBindBuffer(GL_ARRAY_BUFFER, bvhVBO);
	if (this->depthChanged) {
		boundingVerts = this->bvh.boundingBoxes(bvhDepth);
		depthChanged = false;
		std::cout << "----------------------------------------------------" << std::endl;
		for (int i = 0; i < boundingVerts.size(); i += 8)
			std::cout << "{(" << boundingVerts[i].x << "," << boundingVerts[i].y << "),(" << boundingVerts[i + 3].x << "," << boundingVerts[i + 3].y << ")}" << std::endl;
	}
	glBufferData(GL_ARRAY_BUFFER, boundingVerts.size() * sizeof(glm::vec4), static_cast<const void*>(boundingVerts.data()), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glDrawArrays(GL_LINES, 0, boundingVerts.size());
}


void MonteCarlo::drawBVH(){
	GLuint boundingPointsVBO;
	glGenBuffers(1, &boundingPointsVBO);

	std::vector<glm::vec4> points = std::vector<glm::vec4>();
	for (VertexAttrib& v : boundingPoints) {
		points.push_back(v.pos);
	}

	glUseProgram(this->bvhDebugBoundingsShader.graphicsID);
	glBindBuffer(GL_ARRAY_BUFFER, boundingPointsVBO);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec4), static_cast<const void*>(points.data()), GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glDrawArrays(GL_POINTS, 0, points.size());

	drawBVHBox();

	//std::vector<glm::vec4> dists = bvh.distanceBetweenChildren();
	//glUniform4f(loc, 1.0f, 0.0f, 1.0f, 1.0f);
	//glBufferData(GL_ARRAY_BUFFER, dists.size() * sizeof(glm::vec4), static_cast<const void*>(dists.data()), GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	//glDrawArrays(GL_LINES, 0, dists.size());

}

void MonteCarlo::draw() {
	if (isDebugMode) {
		drawBVH();
		return;
	}	

	drawBVHBox();

	if (!isPaused) {
		glUseProgram(this->monteCarloShader.computeID);
		glDispatchCompute(static_cast<GLuint>(this->screenWidth / 32), static_cast<GLuint>(this->screenHeight / 32), static_cast<GLuint>(1u));
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_TEXTURE_UPDATE_BARRIER_BIT);


		//std::cout << opts.pass << " ";
		this->opts.pass++;
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(AlgorithmOpts), nullptr, GL_DYNAMIC_DRAW);
		glCreateBuffers(1, &this->algoSSBO);
		glNamedBufferStorage(this->algoSSBO, sizeof(AlgorithmOpts), static_cast<const void*>(&this->opts), GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->algoSSBO);
	}
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glUseProgram(this->monteCarloShader.graphicsID);
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(this->textureCorners.size()));
}

void MonteCarlo::getValueAtMouse(int x, int y) {
	std::vector<float> tmp = std::vector<float>();
	tmp.resize(static_cast<size_t>(this->screenHeight) * this->screenWidth * 4ull);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, static_cast<void*>(tmp.data()));
	size_t pixIt = 0ull;
	for (size_t i = 0ull; i < tmp.size(); i += 4) {
		pixels[pixIt][0] = tmp[i];
		pixels[pixIt][1] = tmp[i + 1];
		pixels[pixIt][2] = tmp[i + 2];
		pixels[pixIt][3] = tmp[i + 3];
		pixIt++;
	}

	glm::vec4& seeked = pixels[x + static_cast<size_t>(y) * this->screenWidth];

	std::cout << "(" << seeked[0] / seeked[3] << "," << seeked[1] / seeked[3] << "," << seeked[2] / seeked[3] << "," << seeked[3] << ")" << std::endl;
}


void MonteCarlo::printDistanceBetweenChildren(const Node& root) {
	//if (root.itemCount != 0) return;
	//Node& leftChild = bvh.nodes[root.leftChild];
	//Node& rightChild = bvh.nodes[root.rightChild];
	//std::cout << glm::length((leftChild.bb.lowerBound + leftChild.bb.upperBound) / 2.0f - (rightChild.bb.lowerBound + rightChild.bb.upperBound) / 2.0f) << std::endl;
	//printDistanceBetweenChildren(leftChild);
	//printDistanceBetweenChildren(rightChild);
}

void MonteCarlo::generateBVH() {
	this->bvh.buildBVH(this->boundingPoints);
	this->bvhToGPU = this->bvh.bvhNodesToGPUNodes();
	this->indices = this->bvh.bvhIndexToGPUIndex();
	std::vector<BVHNode> nodes = this->bvh.getBVHNodes();
	for (const BVHNode& node : nodes) {
		std::cout << "{(" << node.aabbMin.x << "," << node.aabbMin.y << "),(" << node.aabbMax.x << "," << node.aabbMax.y << ")}" << std::endl;
	}

	int i = 0;
	for (const gpuBVHIndex& idx : indices) {
		std::cout << "[" << i++ << "]. " << idx.index << std::endl;
	}

	std::cout << bvh.getMaxDepth() << std::endl;
}

void MonteCarlo::reset(){
	glGenTextures(1, &this->monteCarloTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->monteCarloTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->screenWidth, this->screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, this->monteCarloTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->monteCarloTexture);

	this->opts.pass = 1;
}


void MonteCarlo::setParams(const MonteCarloParameters& params) {
	this->params = params;
}

void MonteCarlo::cpySSBOStoGPU() {
	glNamedBufferStorage(this->boundarySSBO, sizeof(VertexAttrib) * this->boundingPoints.size(),
		static_cast<const void*>(this->boundingPoints.data()), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->boundarySSBO);

	glNamedBufferStorage(this->paramSSBO, sizeof(MonteCarloParameters), static_cast<const void*>(&this->params), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->paramSSBO);

	glNamedBufferStorage(this->bvhSSBO, sizeof(gpuBVHNode) * this->bvhToGPU.size(), static_cast<const void*>(this->bvhToGPU.data()), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, this->bvhSSBO);

	glNamedBufferStorage(this->indexSSBO, sizeof(gpuBVHIndex) * this->indices.size(), static_cast<const void*>(this->indices.data()), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->indexSSBO);

	glNamedBufferStorage(this->algoSSBO, sizeof(AlgorithmOpts), static_cast<const void*>(&this->opts), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->algoSSBO);
}

void MonteCarlo::setupMonteCarlo(const MonteCarloParameters& params, int height, int width){
	this->setupShaders();
	this->setParams(params);
	this->setScreenDim(height, width);
	this->textureCorners.emplace_back(glm::vec4(this->screenWidth, 0, 0, 1), glm::vec4(0, 0, 0, 1));
	this->textureCorners.emplace_back(glm::vec4(this->screenWidth, this->screenHeight, 0, 1), glm::vec4(0, 0, 0, 1));
	this->textureCorners.emplace_back(glm::vec4(0, this->screenHeight, 0, 1), glm::vec4(0, 0, 0, 1));
	this->textureCorners.emplace_back(glm::vec4(0, 0, 0, 1), glm::vec4(0, 0, 0, 1));

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glCreateBuffers(1, &this->boundarySSBO);
	glCreateBuffers(1, &this->paramSSBO);
	glCreateBuffers(1, &this->windowUBO);
	glCreateBuffers(1, &this->indexSSBO);
	glCreateBuffers(1, &this->bvhSSBO);
	glCreateBuffers(1, &this->algoSSBO);
	glCreateBuffers(1, &this->textureCornerSSBO);

	glm::vec4 resVec(this->screenWidth, this->screenHeight, 0., 0.);
	glNamedBufferStorage(this->windowUBO, sizeof(glm::vec4), static_cast<const void*>(& resVec), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->windowUBO);

	glNamedBufferStorage(this->textureCornerSSBO, sizeof(VertexAttrib) * this->textureCorners.size(), static_cast<const void*>(this->textureCorners.data()), GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, this->textureCornerSSBO);

	float tex[] = {
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

	glGenTextures(1, &this->monteCarloTexture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->monteCarloTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->screenWidth, this->screenHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindImageTexture(0, this->monteCarloTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->monteCarloTexture);
	this->opts._padding1 = 0;
	this->opts.pass = 1;
	this->pixels.resize(static_cast<size_t>(this->screenHeight) * this->screenWidth);
}

void MonteCarlo::setScreenDim(int height, int width) {
	this->screenHeight = height;
	this->screenWidth = width;
}
