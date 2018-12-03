#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__
#include "Component.h"
#include "MathGeoLib.h"
class ComponentMesh :
	public Component
{
public:
	ComponentMesh(const GameObject* dad);
	~ComponentMesh();
	bool CleanUp() override;

	//data structures
	struct myMesh {
		unsigned vbo = 0;
		unsigned vio = 0;
		unsigned material = 0;
		unsigned numVertices = 0;
		unsigned numIndices = 0;
	};

	//vars
	myMesh mesh;
};
#endif
