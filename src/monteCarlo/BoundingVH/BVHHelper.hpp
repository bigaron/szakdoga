#ifndef BOUNDINGVH_BVHHELPER_HPP
#define BOUNDINGVH_BVHHELPER_HPP

#include <glm/glm.hpp>
#include <stdint.h>

struct BVHNode {
	glm::vec4 aabbMin, aabbMax;
	uint32_t leftChild, pointCount;
	bool isLeaf() const { return pointCount > 0; }
};

struct gpuBVHNode {
	glm::vec4 aabbMin, aabbMax;
	int leftChild, pointCount;
	int padding1, padding2;
};

struct gpuBVHIndex {
	int index, pass;
	int padding1, padding2;
};
#endif