#include "ComponentMaterial.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTextures.h"
#include "Application.h"
#include "Serializer.h"

ComponentMaterial::ComponentMaterial(GameObject* dad):
	Component(dad)
{
	type = MATERIAL;
	active = true;
	id = App->generateID();
}

ComponentMaterial::ComponentMaterial(JSON_Value* matFile, GameObject* dad) :
	Component(dad) {
	type = MATERIAL;
	active = matFile->getBool("Active");
	id = matFile->getUint("ID");
	numModel = matFile->getInt("numModel");
	numMaterial = matFile->getInt("numMaterial");
	setPath(matFile->getString("path"));
	isTexture = matFile->getBool("isTexture");
	if (!isTexture) {
		surface.ambientColor = matFile->getVector4("Ambient color");
		surface.difuseColor = matFile->getVector4("Difuse color");
		surface.specularColor = matFile->getVector4("Specular color");
		surface.emisiveColor = matFile->getVector4("Emissive color");
	}

	App->modelLoader->GenerateOneMaterialData(this);
}

ComponentMaterial::~ComponentMaterial()
{
	App->textures->Unload(material.texture0);
}

bool ComponentMaterial::CleanUp() {
	App->textures->Unload(material.texture0);
	delete path;
	return true;
}

void ComponentMaterial::saveMaterial(JSON_Value* val) {
	JSON_Value* mat = val->createValue();
	//mat->convertToArray();

	mat->addUint("ID", id);
	mat->addInt("Type", type);
	mat->addBool("Active", active);
	mat->addInt("numModel", numModel);
	mat->addInt("numMaterial", numMaterial);
	mat->addString("path", path);
	mat->addBool("isTexture", isTexture);
	if (!isTexture) {
		mat->addVector4("Ambient color", surface.ambientColor);
		mat->addVector4("Difuse color", surface.difuseColor);
		mat->addVector4("Specular color", surface.specularColor);
		mat->addVector4("Emissive color", surface.emisiveColor);
	}
	

	val->addValue("Material", mat);
}

void ComponentMaterial::setPath(const char* path) {
	char* copy = new char[strlen(path)];
	strcpy(copy, path);
	this->path = copy;
}