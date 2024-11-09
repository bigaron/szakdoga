#include "BVH.hpp"

void BVH::updateNodeBB(const unsigned int& nodeIndex) {
	Node& currentNode = nodes[nodeIndex];
	float inf = 1e30f;
	currentNode.bb.lowerBound = glm::vec4(inf, inf, 0, 1);
	currentNode.bb.upperBound = glm::vec4(-inf, -inf, 0, 1);
	for (unsigned int pointIdx = currentNode.itemStart; pointIdx < (currentNode.itemStart + currentNode.itemCount); ++pointIdx) {
		glm::vec4& currentPoint = points[pointIndex[pointIdx]].pos;
		currentNode.bb.lowerBound = glm::min(currentNode.bb.lowerBound, currentPoint);
		currentNode.bb.upperBound = glm::max(currentNode.bb.upperBound, currentPoint);
	}
}

void BVH::subdivide(const unsigned int& nodeIndex) {
	Node& currentNode = nodes[nodeIndex];
	if (currentNode.itemCount <= ITEMLIMIT) return;
	glm::vec3 dist = currentNode.bb.upperBound - currentNode.bb.lowerBound;
	int axis = 0;
	if (dist.y > dist.x) axis = 1;
	if (dist.z > dist.y) axis = 2;
	float splitCoord = currentNode.bb.lowerBound[axis] + dist[axis] * 0.5f;
	int i = currentNode.itemStart;
	int j = i + currentNode.itemCount - 1;
	while (i <= j) {
		if (points[pointIndex[i]].pos[axis] < splitCoord) i++;
		else std::swap(pointIndex[i], pointIndex[j--]);
	}

	int leftCount = i - currentNode.itemStart;
	if (leftCount == 0 || leftCount == currentNode.itemCount) return;
	int leftChildIdx = nodesUsed++;
	int rightChildIdx = nodesUsed++;

	nodes[leftChildIdx].itemStart = currentNode.itemStart;
	nodes[leftChildIdx].itemCount = leftCount;
	nodes[rightChildIdx].itemStart = i;
	nodes[rightChildIdx].itemCount = currentNode.itemCount - leftCount;
	currentNode.leftChild = leftChildIdx;
	currentNode.rightChild = rightChildIdx;
	currentNode.itemCount = 0;
	updateNodeBB(leftChildIdx);
	updateNodeBB(rightChildIdx);

	subdivide(leftChildIdx);
	subdivide(rightChildIdx);
}

void BVH::generateBVH() {
	nodes.clear();
	nodes.resize(nodeCount);

	for (unsigned int i = 0u; i < nodeCount; ++i) {
		pointIndex[i] = nodeIndex[i] = i;
	}

	Node& root = nodes[rootIndex];
	root.leftChild = root.rightChild = 0u;
	root.itemStart = 0u;
	root.itemCount = static_cast<unsigned int>(points.size());
	nodesUsed = 1u;
	updateNodeBB(rootIndex);
	subdivide(rootIndex);
}

void BVH::generateBVH(std::vector<VertexAttrib>& points) {
	this->points = points;
	nodeCount = static_cast<unsigned int>(2 * points.size() - 1);
	pointIndex.resize(nodeCount);
	nodeIndex.resize(nodeCount);
	generateBVH();
}

std::vector<glm::vec4> BVH::boundingBoxes() {
	std::vector<glm::vec4> boxCoords = std::vector<glm::vec4>(nodeCount * 8);
	size_t i = 0ull;
	for (const Node& node : nodes) {
		//if (node.itemCount == 0) continue;
		boxCoords[i++] = node.bb.lowerBound;
		boxCoords[i++] = glm::vec4(node.bb.upperBound[0], node.bb.lowerBound[1], 0, 1);
		boxCoords[i++] = glm::vec4(node.bb.upperBound[0], node.bb.lowerBound[1], 0, 1);
		boxCoords[i++] = node.bb.upperBound;
		boxCoords[i++] = node.bb.upperBound;
		boxCoords[i++] = glm::vec4(node.bb.lowerBound[0], node.bb.upperBound[1], 0, 1);
		boxCoords[i++] = glm::vec4(node.bb.lowerBound[0], node.bb.upperBound[1], 0, 1);
		boxCoords[i++] = node.bb.lowerBound;
	}

	boxCoords.resize(i);

	return boxCoords;
}

unsigned int BVH::distinctValues(const std::vector<glm::vec4>& points) {
	std::vector<glm::vec4> used;

	for (unsigned int i = 0; i < points.size(); i += 8) {
		bool skip = false;
		if (std::count(used.begin(), used.end(), points[i]) == 0) used.push_back(points[i]);
		if (std::count(used.begin(), used.end(), points[i + 3]) == 0) used.push_back(points[i + 3]);
	}


	return used.size();
}

std::vector<hostBVH> BVH::hostBVHToDeviceBVH() {
	std::vector<hostBVH> ret(nodeCount);
	hostBVH tmp;
	for (size_t i = 0; i < nodeCount; ++i) {
		const Node& node = nodes[i];
		tmp.pointIndex = static_cast<int>(pointIndex[i]);
		tmp.node = node;
		ret[i] = tmp;
	}

	return ret;
}

std::vector<hostPointIndex> BVH::hostIndicesToDevice() {
	std::vector<hostPointIndex> indices(nodeCount);

	for (unsigned int i = 0; i < nodeCount; ++i) {
		hostPointIndex tmp;
		tmp.pointIndex = pointIndex[i];
		indices[i] = tmp;
	}

	return indices;
}
