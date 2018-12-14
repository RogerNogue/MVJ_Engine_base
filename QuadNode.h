#ifndef __QuadNode_H__
#define __QuadNode_H__
#include "MathGeoLib.h"
#include <vector>
class GameObject;
class QuadNode
{
public:
	QuadNode(float3 min, float3 max, QuadNode* parent);
	QuadNode(float3 min, float3 max);
	~QuadNode();
	void addNode(float3 min, float3 max);

	//variables
	bool rootNode = false;
	float3 minPointArea;
	float3 maxPointArea;
	QuadNode* parent = nullptr;
	//north west child
	QuadNode* child1 = nullptr;
	//north east child
	QuadNode* child2 = nullptr;
	//south east child
	QuadNode* child3 = nullptr;
	//south west child
	QuadNode* child4 = nullptr;
	std::vector <GameObject*> belongings;
};
#endif