#ifndef __ComponentShape_H__
#define __ComponentShape_H__

#include "Component.h"
class JSON_Value;
class par_shapes_mesh;
class ComponentShape :
	public Component
{
public:
	ComponentShape(GameObject* dad, shape_type type);
	ComponentShape(JSON_Value* shapeFile, GameObject* dad);
	~ComponentShape();
	bool CleanUp()override;
	void saveShape(JSON_Value* val);

	//variables
	shape_type shapeType;
	unsigned slices = 20;
	unsigned stacks = 20;
	float size1 = 1;
	float size2 = 2;
	unsigned vbo = 0;
	unsigned vio = 0;
	unsigned material = 0;
	unsigned numVertices = 0;
	unsigned numIndices = 0;
	unsigned texCoords_offset = 0;
	unsigned normals_offset = 0;

private:
	par_shapes_mesh* mesh = nullptr;

};
#endif
