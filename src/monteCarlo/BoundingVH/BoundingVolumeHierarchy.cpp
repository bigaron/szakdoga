#include "BoundingVolumeHierarchy.hpp"

void BoundingVH::updateNodeBounds(size_t idx) {
	BVHNode& node = bvhNodes[idx];
	node.aabbMin = glm::vec4(inf);
	node.aabbMax = glm::vec4(-inf);

	for (uint32_t first = node.leftChild, i = 0; i < node.pointCount; ++i) {
		VertexAttrib& point = boundingPoints[pointIdx[first + i]];
		node.aabbMax = glm::max(node.aabbMax, point.pos);
		node.aabbMin = glm::min(node.aabbMin, point.pos);
	}
}

void BoundingVH::subdivide(size_t idx) {
	BVHNode& node = bvhNodes[idx];
	if (node.pointCount <= 2) return;
	glm::vec4 extent = node.aabbMax - node.aabbMin;
	int axis = 0;
	if (extent.y > extent.x) axis = 1;
	if (extent.z > extent[axis]) axis = 2;
	float splitPos = node.aabbMin[axis] + extent[axis] * 0.5f;

	uint32_t i = node.leftChild;
	uint32_t j = i + node.pointCount - 1;
	while (i <= j) {
		if (boundingPoints[pointIdx[ i ]].pos[axis] < splitPos) i++;
		else std::swap(pointIdx[i], pointIdx[j--]);
	}

	uint32_t leftCount = i - node.leftChild;
	if (leftCount == 0 || leftCount == node.pointCount) return;
	uint32_t leftChildIdx = nodesUsed++;
	uint32_t rightChildIdx = nodesUsed++;
	bvhNodes[leftChildIdx].leftChild = node.leftChild;
	bvhNodes[leftChildIdx].pointCount = leftCount;
	bvhNodes[rightChildIdx].leftChild = i;
	bvhNodes[rightChildIdx].pointCount = node.pointCount - leftCount;
	node.leftChild = leftChildIdx;
	node.pointCount = 0;

	updateNodeBounds(leftChildIdx);
	updateNodeBounds(rightChildIdx);
	subdivide(leftChildIdx);
	subdivide(rightChildIdx);
}


void BoundingVH::buildBVH(const std::vector<VertexAttrib>& points) {
	boundingPoints = points;
	maxNodeCount = points.size() * 2;
	buildBVH();
}

void BoundingVH::buildBVH() {
	nodesUsed = 1u;
	pointIdx.resize(boundingPoints.size());
	bvhNodes.resize(maxNodeCount);
	for (uint32_t i = 0u; i < boundingPoints.size(); ++i) {
		pointIdx[i] = i;
	}

	BVHNode& root = bvhNodes[rootNodeIdx];
	root.leftChild = 0;
	root.pointCount = boundingPoints.size();

	updateNodeBounds(rootNodeIdx);
	subdivide(rootNodeIdx);
}

std::vector<gpuBVHNode> BoundingVH::bvhNodesToGPUNodes() {
	std::vector<gpuBVHNode> gpuNodes(bvhNodes.size());

	gpuBVHNode tmp = gpuBVHNode();
	for (const BVHNode& node : bvhNodes) {
		tmp.aabbMin = node.aabbMin;
		tmp.aabbMax = node.aabbMax;
		tmp.leftChild = node.leftChild;
		tmp.pointCount = node.pointCount;
		gpuNodes.push_back(tmp);
	}

	return gpuNodes;
}