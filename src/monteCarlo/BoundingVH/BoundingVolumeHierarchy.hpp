#ifndef BOUNDINGVH_BOUNDINGVOLUMEHIERARCHY_HPP
#define BOUNDINGVH_BOUNDINGVOLUMEHIERARCHY_HPP

#include <vector>
#include <stdint.h>

#include "../vertexAttrib.hpp"
#include "BVHHelper.hpp"

class BoundingVH {
	const float inf = 1e30f;
	std::vector<VertexAttrib> boundingPoints;
	std::vector<BVHNode> bvhNodes;
	std::vector<uint32_t> pointIdx;
	const size_t maxNodeCount, rootNodeIdx = 0ull;
	uint32_t nodesUsed;

	void updateNodeBounds(size_t idx);
	void subdivide(size_t idx);
public:
	BoundingVH(const std::vector<VertexAttrib>& boundingPoints) : boundingPoints(boundingPoints), maxNodeCount(2*boundingPoints.size()){
		nodesUsed = 1u;
	}

	void buildBVH();

};

#endif // !BoundingVolumeHierarchy
