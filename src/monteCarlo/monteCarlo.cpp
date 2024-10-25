#include "monteCarlo.hpp"

void MonteCarlo::readInputFromFile(const char* filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "The file at the given filepath: [" << filePath << "] could not be opened!" << std::endl << "Returning..." << std::endl;
		return;
	}

	std::string line;
	size_t posDim, colDim;
	char valueSep = ' ', typeSep = ';', inHouseSep = ' ';
	
	std::getline(file, line);
	std::vector<std::string> dimType = HelperFunctions::split(line, " ");
	if (dimType.size() < 2) {
		std::cerr << "The file format specified is not correct. Please consult the README file for the proper format. " << std::endl <<  "Returning..." << std::endl;
		return;
	}

	posDim = std::stoull(dimType[0]);
	colDim = std::stoull(dimType[1]);

	std::istream& ret = std::getline(file, line);
	//We check if the input was formatted correctly
	if (ret.fail()) {
		std::cerr << "The file format specified is not correct. Please consult the README file for the proper format. " << std::endl << "Returning..." << std::endl;
		return;
	}



	while (std::getline(file, line)) {
		
	}
}
