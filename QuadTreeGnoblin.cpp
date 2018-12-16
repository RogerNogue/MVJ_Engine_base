#include "QuadTreeGnoblin.h"
#include "QuadNode.h"



QuadTreeGnoblin::QuadTreeGnoblin()
{
}


QuadTreeGnoblin::~QuadTreeGnoblin()
{
}

void QuadTreeGnoblin::Create(AABB limits) {
	active = true;
	rootNode = new QuadNode(limits);
}

void QuadTreeGnoblin::Clear() {

}

void QuadTreeGnoblin::CollectIntersect(std::vector<GameObject*>& objects, QuadNode& node) {
	node.getIntersections(objects);
}

void QuadTreeGnoblin::Insert(GameObject* obj) {
	rootNode->addNode(obj);
}