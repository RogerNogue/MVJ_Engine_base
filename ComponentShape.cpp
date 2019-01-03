#include "ComponentShape.h"
#include "Application.h"
#include "Serializer.h"
#include "par_shapes.h"
#include "GameObject.h"
#include "ComponentMaterial.h"

ComponentShape::ComponentShape(GameObject* dad, shape_type type):
Component(dad),
shapeType(type)
{
	this->type = SHAPE;
	active = true;
	id = App->generateID();
}

ComponentShape::ComponentShape(JSON_Value* shapeFile, GameObject* dad) :
	Component(dad) {
	type = SHAPE;
	active = shapeFile->getBool("Active");
	id = shapeFile->getUint("ID");
	slices = shapeFile->getUint("Slices");
	stacks = shapeFile->getUint("Stacks");
	
}

ComponentShape::~ComponentShape()
{
}

bool ComponentShape::CleanUp() {
	
	return true;
}

void ComponentShape::saveShape(JSON_Value* val) {
	JSON_Value* shapeval = val->createValue();
	//shapeval->convertToArray();

	shapeval->addUint("ID", id);
	shapeval->addInt("Type", type);
	shapeval->addBool("Active", active);
	shapeval->addUint("Slices", slices);
	shapeval->addUint("Stacks", stacks);
	
	

	val->addValue("Shape", shapeval);
}