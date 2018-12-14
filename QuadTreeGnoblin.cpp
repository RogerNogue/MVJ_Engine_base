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
	rootNode = new QuadNode(limits.minPoint, limits.maxPoint);
}

void QuadTreeGnoblin::Clear() {

}

void CollectIntersect(std::vector<GameObject*>& objects, QuadNode& node) {

}