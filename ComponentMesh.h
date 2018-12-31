#ifndef __ComponentMesh_H__
#define __ComponentMesh_H__
#include "Component.h"
#include "MathGeoLib.h"
class JSON_Value;
class ComponentMesh :
	public Component
{
public:
	ComponentMesh(GameObject* dad);
	ComponentMesh(JSON_Value* mesh, GameObject* dad);
	~ComponentMesh();
	bool CleanUp() override;
	void saveMesh(JSON_Value* val);

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
