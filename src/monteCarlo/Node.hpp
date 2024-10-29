#ifndef MONTECARLO_NODE_HPP
#define MONTECARLO_NODE_HPP

#include "glm/glm.hpp"

struct BoundingBox {
	//lowerBound is treated as the bottom left corner
	//upperBound is treated as the top right corner
	glm::vec4 lowerBound, upperBound;

	BoundingBox(glm::vec4 lower, glm::vec4 upper) {
		lowerBound = lower;
		upperBound = upper;
	}

	BoundingBox() {
		lowerBound = upperBound = glm::vec4(-1, -1, -1, -1);
	}

	bool isPointInBox(const glm::vec4& point) {
		return point.x > lowerBound.x && point.x < upperBound.x && point.y > lowerBound.y && point.y < upperBound.y;
	}
};

struct Node {
	BoundingBox bb;
	unsigned int leftChild, rightChild;
	unsigned int itemStart, itemCount;
};


#endif // !MONTECARLO_NODE_HPP
