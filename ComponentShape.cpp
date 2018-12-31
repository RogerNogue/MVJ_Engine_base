#include "ComponentShape.h"
#include "Application.h"
#include "Serializer.h"


ComponentShape::ComponentShape(GameObject* dad):
Component(dad)
{
	type = SHAPE;
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
	color = shapeFile->getVector4("Color");
}

ComponentShape::~ComponentShape()
{
}

bool ComponentShape::CleanUp() {
	
	return true;
}

void ComponentShape::generateSphere(unsigned slices, unsigned stacks, math::float4* color) {
	this->color.x = color->x; this->color.y = color->y; this->color.z = color->z; this->color.w = color->w;
	this->slices = slices; this->stacks = stacks;
}

void ComponentShape::saveShape(JSON_Value* val) {
	JSON_Value* shapeval = val->createValue();
	//shapeval->convertToArray();

	shapeval->addUint("ID", id);
	shapeval->addInt("Type", type);
	shapeval->addBool("Active", active);
	shapeval->addUint("Slices", slices);
	shapeval->addUint("Stacks", stacks);
	shapeval->addVector4("Color", color);
	

	val->addValue("Shape", shapeval);
}