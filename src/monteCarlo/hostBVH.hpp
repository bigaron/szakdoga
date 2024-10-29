#ifndef MONTECARLO_HOSTBVH_HPP
#define MONTECARLO_HOSTBVH_HPP

#include "Node.hpp"
#include "glm/glm.hpp"

struct hostNode {
	glm::vec4 lowerBound, upperBound;
	int leftChild, rightChild;
	int itemStart, itemCount;
};

struct hostBVH {
	Node node;
	int pointIndex = -1;
	int padding1 = 0, padding2 = 0, padding3 = 0;

	hostBVH() {
		node = Node();
	}
};

struct hostPointIndex {
	int pointIndex;
	int pass, padding2, padding3;
};

#endif // !MONTECARLO_HOSTBVH_HPP
