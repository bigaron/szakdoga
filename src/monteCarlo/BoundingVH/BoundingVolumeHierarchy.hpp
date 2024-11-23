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
	size_t maxNodeCount, rootNodeIdx = 0ull;
	uint32_t nodesUsed;
	std::vector<glm::vec4> layerVerts;


	void updateNodeBounds(size_t idx);
	void subdivide(size_t idx);

	void getNodeIndicesOfLayer(const BVHNode& node, int currentLayer, const int desiredDepth = 0);
public:
	BoundingVH(): maxNodeCount(0), nodesUsed(1) {}
	BoundingVH(const std::vector<VertexAttrib>& boundingPoints) : boundingPoints(boundingPoints), maxNodeCount(2*boundingPoints.size()){
		nodesUsed = 1u;
	}

	void buildBVH();
	void buildBVH(const std::vector<VertexAttrib>& points);
	std::vector<BVHNode> getBVHNodes() { return bvhNodes; }
	std::vector<gpuBVHNode> bvhNodesToGPUNodes();
	std::vector<gpuBVHIndex> bvhIndexToGPUIndex();
	std::vector<glm::vec4> boundingBoxes(int layer = 0);
};

#endif // !BoundingVolumeHierarchy
