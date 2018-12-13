#ifndef __QuadTreeGnoblin_H__
#define __QuadTreeGnoblin_H__
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>
class GameObject;
class QuadNode;
class QuadTreeGnoblin
{
public:
	QuadTreeGnoblin();
	~QuadTreeGnoblin();
	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* obj);
	void CollectIntersect(std::vector<GameObject*>& objects, QuadNode& node);
	
};

#endif

