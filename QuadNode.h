#ifndef __QuadNode_H__
#define __QuadNode_H__
#include "MathGeoLib.h"
#include <vector>
class GameObject;
class QuadNode
{
public:
	QuadNode(AABB section, QuadNode* parent, int num);
	QuadNode(AABB section);
	~QuadNode();
	void addNode(GameObject* node);
	void deleteObject(GameObject* obj);
	void getIntersections(std::vector<GameObject*>& objects);
	void drawTree();
	void CleanUp();

	//variables
	bool rootNode = false;
	AABB area;
	QuadNode* parent = nullptr;
	//north west child
	QuadNode* child1 = nullptr;
	//north east child
	QuadNode* child2 = nullptr;
	//south east child
	QuadNode* child3 = nullptr;
	//south west child
	QuadNode* child4 = nullptr;
	int treeDepth;
	int maxDepth = 50;
	int maxObjectsPerNode = 2;
	std::vector <GameObject*> belongings;
private:
	int childNumber = 0; //to see if its child 1, 2, 3 or 4
};
#endif