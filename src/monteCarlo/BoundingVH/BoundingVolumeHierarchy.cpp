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
	size_t i = 0ull;
	for (const BVHNode& node : bvhNodes) {
		tmp.aabbMin = node.aabbMin;
		tmp.aabbMax = node.aabbMax;
		tmp.leftChild = node.leftChild;
		tmp.pointCount = node.pointCount;
		gpuNodes[i++] = tmp;
	}

	return gpuNodes;
}

std::vector<gpuBVHIndex> BoundingVH::bvhIndexToGPUIndex() {
	std::vector<gpuBVHIndex> gpuIndicies(boundingPoints.size());

	gpuBVHIndex tmp = gpuBVHIndex();
	for (int i = 0; i < pointIdx.size(); ++i) {
		tmp.index = pointIdx[i];
		tmp.pass = tmp.padding1 = tmp.padding2 = -1;
		gpuIndicies[i] = tmp;
	}

	return gpuIndicies;
}

std::vector<glm::vec4> BoundingVH::getNodeIndicesOfLayer(const BVHNode& node, std::vector<glm::vec4> base, int currentLayer, const int desiredDepth) {
	if (currentLayer == desiredDepth) {
		base.push_back(node.aabbMin);
		base.push_back(node.aabbMax);
		return base;
	}
	std::vector<glm::vec4> left = getNodeIndicesOfLayer(bvhNodes[node.leftChild], base, currentLayer + 1, desiredDepth);
	std::vector<glm::vec4> right = getNodeIndicesOfLayer(bvhNodes[node.leftChild + 1], base, currentLayer + 1, desiredDepth);

	for (glm::vec4 val : left) base.push_back(val);
	for (glm::vec4 val : right) base.push_back(val);
	return base;
}

std::vector<glm::vec4> BoundingVH::boundingBoxes(int layer) {
	std::vector<glm::vec4> boxes, result;

	if (layer != 0) boxes = getNodeIndicesOfLayer(bvhNodes[rootNodeIdx], boxes, 1, layer);


	for (int i = 0; i < boxes.size(); i += 2) {
		result.push_back(boxes[i]);
		result.push_back(glm::vec4(boxes[i + 1].x, boxes[i].y, 0, 1));
		result.push_back(glm::vec4(boxes[i + 1].x, boxes[i].y, 0, 1));
		result.push_back(boxes[i + 1]);
		result.push_back(boxes[i + 1]);
		result.push_back(glm::vec4(boxes[i].x, boxes[i + 1].y, 0, 1));
		result.push_back(glm::vec4(boxes[i].x, boxes[i + 1].y, 0, 1));
		result.push_back(boxes[i]);
	}

	return result;
}