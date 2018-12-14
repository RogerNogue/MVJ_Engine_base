#include "QuadNode.h"
#include "GameObject.h"


QuadNode::QuadNode(float3 min, float3 max):
	minPointArea(min),
	maxPointArea(max)
{
}

QuadNode::QuadNode(float3 min, float3 max, QuadNode* parent):
minPointArea(min),
maxPointArea(max),
parent(parent),
rootNode(true)
{
}

QuadNode::~QuadNode()
{
}

void QuadNode::addNode(float3 min, float3 max) {
	std::vector<float3> corners;
	corners.push_back(min); corners.push_back(max);
	corners.push_back(float3(min.x, 0, max.z));
	corners.push_back(float3(max.x, 0, min.z));
	//checking in how many of the 4 sections of the area the new item belongs
	//then we either create the new child or repeat the process for 
	//every child it belongs
	bool belongs1, belongs2, belongs3, belongs4;
	belongs1 = belongs2 = belongs3 = belongs4 = false;
	//get the boundaries of each child. If we dont have them, we calculate them
	float3 minArea1, maxArea1, minArea2, maxArea2, minArea3, maxArea3, minArea4, maxArea4;
	float3 center(minPointArea.x + (maxPointArea.x - minPointArea.x)/2, 0, minPointArea.z + (maxPointArea.z - minPointArea.z) / 2);
	if (child1 != nullptr) {
		minArea1 = child1->minPointArea; maxArea1 = child1->maxPointArea;
	}else {
		minArea1 = float3(minPointArea.x, 0, center.z);
		maxArea1 = float3(center.x, 0, maxPointArea.z);
	}
	if (child2 != nullptr) {
		minArea2 = child2->minPointArea; maxArea2 = child2->maxPointArea;
	}else {
		minArea2 = center;
		maxArea2 = float3(maxPointArea.x, 0, maxPointArea.z);
	}
	if (child3 != nullptr) {
		minArea3 = child3->minPointArea; maxArea3 = child3->maxPointArea;
	}else {
		minArea3 = float3(center.x, 0, minPointArea.z);
		maxArea3 = float3(maxPointArea.x, 0, center.z);
	}
	if (child4 != nullptr) {
		minArea4 = child4->minPointArea; maxArea4 = child4->maxPointArea;
	}else {
		minArea4 = float3(minPointArea.x, 0, minPointArea.z);
		maxArea4 = center;
	}
	for (int i = 0; i < corners.size(); ++i) {
		if (!belongs1 && corners[i].x > minArea1.x && corners[i].x < maxArea1.x && corners[i].z > minArea1.y && corners[i].z < maxArea1.x) {
			//if child not created yet
			if (child1 == nullptr) {
				child1 = new QuadNode(minArea1, maxArea1, this);
			}
			belongs1 = true;
		}
		if (!belongs2 && corners[i].x > minArea2.x && corners[i].x < maxArea2.x && corners[i].z > minArea2.y && corners[i].z < maxArea2.x) {
			//if child not created yet
			if (child2 == nullptr) {
				child2 = new QuadNode(minArea2, maxArea2, this);
			}
			belongs2 = true;
		}
		if (!belongs3 && corners[i].x > minArea3.x && corners[i].x < maxArea3.x && corners[i].z > minArea3.y && corners[i].z < maxArea3.x) {
			//if child not created yet
			if (child3 == nullptr) {
				child3 = new QuadNode(minArea3, maxArea3, this);
			}
			belongs3 = true;
		}
		if (!belongs4 && corners[i].x > minArea4.x && corners[i].x < maxArea4.x && corners[i].z > minArea4.y && corners[i].z < maxArea4.x) {
			//if child not created yet
			if (child4 == nullptr) {
				child4 = new QuadNode(minArea4, maxArea4, this);
			}
			belongs4 = true;
		}
	}
}
