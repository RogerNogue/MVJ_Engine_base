#include "QuadNode.h"
#include "GameObject.h"
#include "debugdraw.h"
#include "Application.h"
#include "ModuleMenu.h"

QuadNode::QuadNode(AABB section)
{
	area = AABB(float3(section.minPoint.x, 0, section.minPoint.z), float3(section.maxPoint.x, 0, section.maxPoint.z));
	treeDepth = 1;
}

QuadNode::QuadNode(AABB section, QuadNode* parent, int num):
parent(parent),
rootNode(true),
childNumber(num)
{
	area = AABB(float3(section.minPoint.x, 0, section.minPoint.z), float3(section.maxPoint.x, 0, section.maxPoint.z));
	treeDepth = parent->treeDepth + 1;
	/*char* b = new char[100];
	sprintf(b, "Creating new quad node with depth = %d\n", treeDepth);
	App->menu->console.AddLog(b);
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;*/
}

QuadNode::~QuadNode()
{
}

void QuadNode::CleanUp() {

}

void QuadNode::addNode(GameObject* node) {
	//case tree is already at max depth:
	if (treeDepth >= maxDepth) {
		belongings.push_back(node);
		node->nodesItAppears.push_back(this);
		return;
	}
	AABB area1, area2, area3, area4;
	float3 center = area.CenterPoint();
	//checking in how many of the 4 sections of the area the new item belongs
	if (child1 != nullptr) {
		area1 = child1->area;
	}else {
		area1 = AABB(float3(area.minPoint.x, 0, center.z), float3(center.x, 0, area.maxPoint.z) );
	}
	if (child2 != nullptr) {
		area2 = child2->area;
	}else {
		area2 = AABB(center, area.maxPoint);
	}
	if (child3 != nullptr) {
		area3 = child3->area;
	}else {
		area3 = AABB(float3(center.x, 0, area.minPoint.z), float3(area.maxPoint.x, 0, center.z));
	}
	if (child4 != nullptr) {
		area4 = child4->area;
	}else {
		area4 = AABB(area.minPoint, center);
	}
	/*
	now that the 4 areas are defined, the object will be intersected with
	each one of them and then treated according the situation:
	- if does not intersect, nothing is done
	- if intersects and is not leaf, get into the leaf recursively before doing next checks
	- if intersects and is leaf, check if leaf is full: if isn't just place the item reference
			if it is full, create a new leaf. Also keep in mint maxDepth value
	*/
	if (node->boundingBox.Intersects(area1)) {
		//intersects, checking if it can just be placed and leave
		if (belongings.size() < maxObjectsPerNode) {
			//case we can just place object and leave
			belongings.push_back(node);
			node->nodesItAppears.push_back(this);
		}
		else if (child1 == nullptr) {
			//if not, we just create a child and place node there
			child1 = new QuadNode(area1, this, 1);
			child1->addNode(node);
		}else {
			//if has a child, we just tell it to add the object
			child1->addNode(node);
		}
	}
	//and do the same for each section
	//section2
	if (node->boundingBox.Intersects(area2)) {
		if (child2 == nullptr) {
			if (belongings.size() < maxObjectsPerNode) {
				belongings.push_back(node);
				node->nodesItAppears.push_back(this);
			}
			else {
				child2 = new QuadNode(area2, this, 2);
				child2->addNode(node);
			}
		}
		else {
			child2->addNode(node);
		}
	}
	//section3
	if (node->boundingBox.Intersects(area3)) {
		if (child3 == nullptr) {
			if (belongings.size() < maxObjectsPerNode) {
				belongings.push_back(node);
				node->nodesItAppears.push_back(this);
			}
			else {
				child3 = new QuadNode(area3, this, 3);
				child3->addNode(node);
			}
		}
		else {
			child3->addNode(node);
		}
	}
	//section4
	if (node->boundingBox.Intersects(area4)) {
		if (child4 == nullptr) {
			if (belongings.size() < maxObjectsPerNode) {
				belongings.push_back(node);
				node->nodesItAppears.push_back(this);
			}
			else {
				child4 = new QuadNode(area4, this, 4);
				child4->addNode(node);
			}
		}
		else {
			child4->addNode(node);
		}
	}
}

void QuadNode::deleteObject(GameObject* obj) {
	for (int i = 0; i < belongings.size(); ++i) {
		if (belongings[i]->id == obj->id) belongings.erase(belongings.begin() + i);
	}
	if (belongings.size() == 0 && child1 == nullptr && child2 == nullptr && child3 == nullptr && child4 == nullptr) {
		belongings.clear();
		switch (childNumber) {
		case 1:
			CleanUp();
			parent->deletechild1();
			break;
		case 2:
			CleanUp();
			parent->deletechild2();
			break;
		case 3:
			CleanUp();
			parent->deletechild3();
			break;
		case 4:
			CleanUp();
			parent->deletechild4();
			break;
		}
	}
}

void QuadNode::getIntersections(std::vector<GameObject*>& objects) {
	for (int i = 0; i < belongings.size(); ++i) {
		objects.push_back(belongings[i]);
	}
}

void QuadNode::drawTree() {
	const ddVec3 treeColor = { 0.4f, 0.4f, 0.8f };
	float3 minpoint(area.minPoint.x, -10000, area.minPoint.z);
	float3 maxpoint(area.maxPoint.x, 10000, area.maxPoint.z);
	dd::aabb(minpoint, maxpoint, treeColor);

	if (child1 != nullptr) child1->drawTree();
	if (child2 != nullptr) child2->drawTree();
	if (child3 != nullptr) child3->drawTree();
	if (child4 != nullptr) child4->drawTree();
}

void QuadNode::deletechild1() {
	delete child1;
	child1 = nullptr;
}
void QuadNode::deletechild2() {
	delete child2;
	child2 = nullptr;
}
void QuadNode::deletechild3() {
	delete child3;
	child3 = nullptr;
}
void QuadNode::deletechild4() {
	delete child4;
	child4 = nullptr;
}