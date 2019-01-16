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
	void setPath(const char* path);

	//data structures
	struct myMesh {
		unsigned vbo = 0;
		unsigned vio = 0;
		unsigned material = 0;
		unsigned numVertices = 0;
		unsigned numIndices = 0;
		unsigned normalsOffset = 0;
		unsigned texCoordsOffset = 0;
		bool normals = false;
	};

	//vars
	myMesh mesh;
	float ambient = 0.3f;
	int numModel = 0;
	int numMesh = 0;
	const char* path = "";
};
#endif
