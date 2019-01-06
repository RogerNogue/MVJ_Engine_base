#include "ComponentShape.h"
#include "Application.h"
#include "Serializer.h"
#include "par_shapes.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "imgui.h"
#include "ModuleModelLoader.h"

ComponentShape::ComponentShape(GameObject* dad, shape_type type):
Component(dad),
shapeType(type)
{
	dad->shape = this;
	dad->hasShape = true;
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
	
	//gotta test these 2 instructions
	dad->shape = this;
	dad->hasShape = true;
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

void ComponentShape::drawEditorMenu() {
	
	switch (shapeType) {
	case SPHERE:
		SphereEditor();
		break;

	case CUBE:
		CubeEditor();
		break;

	case CYLINDER:
		CylinderEditor();
		break;

	case TORUS:
		TorusEditor();
		break;
	}		
	
}

void ComponentShape::SphereEditor() {
	if (ImGui::CollapsingHeader("Sphere values"))
	{
		ImGui::PushID("1");
		ImGui::SliderInt("Slices", &slices, 0, 40);
		ImGui::PopID();
		ImGui::PushID("2");
		ImGui::SliderInt("Stacks", &stacks, 0, 40);
		ImGui::PopID();
		ImGui::PushID("3");
		ImGui::SliderFloat("Radius", &size1, 0.1f, 50.0f);
		ImGui::PopID();

		if (ImGui::Button("Re-generate sphere")) {
			//store the material, delete everything generated
			//and re do the mode loader process with the new values.
			App->modelLoader->LoadSphere(this, false);
		}
	}
}

void ComponentShape::CubeEditor() {
	if (ImGui::CollapsingHeader("Cube values"))
	{
		ImGui::PushID("1");
		ImGui::SliderFloat("Size_X", &size1, 0, 200);
		ImGui::PopID();
		ImGui::PushID("2");
		ImGui::SliderFloat("Size_Y", &size2, 0, 200);
		ImGui::PopID();
		ImGui::PushID("3");
		ImGui::SliderFloat("Size_Z", &size3, 0, 200);
		ImGui::PopID();

		if (ImGui::Button("Re-generate cube")) {
			//store the material, delete everything generated
			//and re do the mode loader process with the new values.
			App->modelLoader->LoadCube(this, false);
		}
	}
}

void ComponentShape::CylinderEditor() {
	if (ImGui::CollapsingHeader("Cylinder values"))
	{
		ImGui::PushID("1");
		ImGui::SliderFloat("Radius", &size1, 0, 200);
		ImGui::PopID();
		ImGui::PushID("2");
		ImGui::SliderFloat("Height", &size2, 0, 200);
		ImGui::PopID();

		if (ImGui::Button("Re-generate cylinder")) {
			//store the material, delete everything generated
			//and re do the mode loader process with the new values.
			App->modelLoader->LoadCylinder(this, false);
		}
	}
}

void ComponentShape::TorusEditor() {
	if (ImGui::CollapsingHeader("Torus values"))
	{
		ImGui::PushID("1");
		ImGui::SliderInt("Slices", &slices, 0, 40);
		ImGui::PopID();
		ImGui::PushID("2");
		ImGui::SliderInt("Stacks", &stacks, 0, 40);
		ImGui::PopID();
		ImGui::PushID("3");
		ImGui::SliderFloat("Inner radius", &size1, 0.1f, 1.0f);
		ImGui::PopID();
		ImGui::PushID("4");
		ImGui::SliderFloat("Outer radius", &size2, 0.1f, 50.0f);
		ImGui::PopID();

		if (ImGui::Button("Re-generate torus")) {
			//store the material, delete everything generated
			//and re do the mode loader process with the new values.
			App->modelLoader->LoadTorus(this, false);
		}
	}
}