#ifndef MONTECARLO_BVH_HPP
#define MONTECARLO_BVH_HPP

#include <vector>
#include "glm/glm.hpp"
#include "VertexAttrib.hpp"
#include "hostBVH.hpp"
#include "Node.hpp"
#include <cmath>


class BVH {

	void updateNodeBB(const unsigned int& nodeIndex);
	void subdivide(const unsigned int& nodeIndex);

public:
	std::vector<VertexAttrib> points;
	std::vector<unsigned int> pointIndex, nodeIndex;
	std::vector<Node> nodes;
	unsigned int rootIndex = 0u, nodeCount = 0u, nodesUsed = 0u;
	unsigned int ITEMLIMIT = 0;

	BVH(){}

	BVH(std::vector<VertexAttrib>& points) {
		generateBVH(points);
		rootIndex = 0;
		ITEMLIMIT = 2;
	}

	void generateBVH();
	void generateBVH(std::vector<VertexAttrib>& points);

	std::vector<glm::vec4> boundingBoxes();

	unsigned int distinctValues(const std::vector<glm::vec4>& points);
	std::vector<hostBVH> hostBVHToDeviceBVH();
	std::vector<hostPointIndex> hostIndicesToDevice();
};


#endif // !MONTECARLO_BVH_HPP
