#include "GameObject.h"
#include "ComponentMaterial.h"
#include "Component.h"
#include "ModuleCamera.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ComponentCamera.h"
#include "ModuleModelLoader.h"
#include "MathGeoLib.h"
#include "ModuleScene.h"
#include "debugdraw.h"
#include "ModuleDebugDraw.h"
#include "QuadNode.h"
#include "Serializer.h"
#include "ComponentShape.h"

GameObject::GameObject(const char* n){
	char* copyName = new char[strlen(n)];
	strcpy(copyName, n);
	name = copyName;

	active = true;
	id = App->generateID();
	transform = new ComponentTransform(this);	
	minX = minY = minZ = -App->scene->sceneScale;
	maxX = maxY = maxZ = App->scene->sceneScale;
	calculateAABB();
	App->scene->allObjects.push_back(this);
	updateQuadTree();
	parentId = 0;
}

GameObject::GameObject(const char* n, GameObject* parent):
	parent(parent)
{
	char* copyName = new char[strlen(n)];
	strcpy(copyName, n);
	name = copyName;

	active = true;
	parent->children.push_back(this);
	id = App->generateID();
	transform = new ComponentTransform(this);
	minX = minY = minZ = -App->scene->sceneScale;
	maxX = maxY = maxZ = App->scene->sceneScale;
	calculateAABB();
	App->scene->allObjects.push_back(this);
	updateQuadTree();
	parentId = parent->id;
}

GameObject::GameObject(const char* n, GameObject* parent, bool physical) :
	parent(parent),
	Physical(physical)
{
	char* copyName = new char[strlen(n)];
	strcpy(copyName, n);
	name = copyName;

	active = true;
	if (physical) {
		parent->meshesOrShapes.push_back(this);
	}
	else {
		parent->children.push_back(this);
		transform = new ComponentTransform(this);
	}
	minX = minY = minZ = -App->scene->sceneScale;
	maxX = maxY = maxZ = App->scene->sceneScale;
	id = App->generateID();
	App->scene->allObjects.push_back(this);
	parentId = parent->id;
}

GameObject::GameObject(const char* n, GameObject* parent, int signal, bool light) :
	parent(parent),
	light(light)
{
	char* copyName = new char[strlen(n)];
	strcpy(copyName, n);
	name = copyName;

	active = true;
	minX = minY = minZ = -App->scene->sceneScale;
	maxX = maxY = maxZ = App->scene->sceneScale;
	id = App->generateID();
	App->scene->allObjects.push_back(this);
	parentId = parent->id;

	parent->children.push_back(this);
	transform = new ComponentTransform(this);
	calculateAABB();
	App->scene->allObjects.push_back(this);
	
	paintBB = true;
}

GameObject::GameObject(JSON_Value* objValue) {
	id = objValue->getUint("ID");
	parentId = objValue->getUint("ParentID");
	active = objValue->getBool("Is active");
	isStatic = objValue->getBool("Is static");
	paintBB = objValue->getBool("Bounding box painted");
	name = objValue->getString("name");
	hascamera = objValue->getBool("Has camera");
	hasmesh = objValue->getBool("Has mesh");
	hasmaterial = objValue->getBool("Has material");
	hasShape = objValue->getBool("Has shape");
	minX = objValue->getFloat("Min X");
	minY = objValue->getFloat("Min Y");
	minZ = objValue->getFloat("Min Z");
	maxX = objValue->getFloat("Max X");
	maxY = objValue->getFloat("Max Y");
	maxZ = objValue->getFloat("Max Z");
	Physical = objValue->getBool("Physical");
	BBGenerated = objValue->getBool("BBgen");
	light = objValue->getBool("Light");

	JSON_Value* Components = objValue->getValue("Component"); //It is an array of values
	if (Components->getRapidJSONValue()->IsArray()) //Just make sure
	{
		ComponentMaterial* mat = nullptr;
		for (int i = 0; i < Components->getRapidJSONValue()->Size(); i++)
		{
			JSON_Value* componentData = Components->getValueFromArray(i); //Get the component data
			switch (componentData->getInt("Type")) {
			case MESH:
				mesh = new ComponentMesh(componentData, this);
				break;
			case CAMERA:
				camera = new ComponentCamera(componentData, this);
				break;
			case MATERIAL:
				mat = new ComponentMaterial(componentData, this);
				materials.push_back(mat);
				hasmaterial = true;
				break;
			case TRANSFORM:
				transform = new ComponentTransform(componentData, this);
				break;
			case SHAPE:
				shape = new ComponentShape(componentData, this);
				break;
			}
		}
	}
	if (!Physical) calculateAABB();
	if (light) App->scene->allLights.push_back(this);
	else App->scene->allObjects.push_back(this);
}

GameObject::~GameObject()
{
}

void GameObject::deleteObject() {
	char* b = new char[100];
	sprintf(b, "Deleting object: ");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog(name);
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;

	for (int i = 0; i < children.size(); ++i) {
		children[i]->deleteObject();
		//delete children[i];
	}
	if(parent != nullptr) parent->deleteChild(id);
	children.clear();
	if(!Physical){
		for (int k = 0; k < meshesOrShapes.size();) {
			//delete the mesh or shape from object and from AllMeshes and AllShapes
			meshesOrShapes[k]->deleteObject();
			//meshesOrShapes.erase(meshesOrShapes.begin()+k);
		}
		meshesOrShapes.clear();
	}else {
		if (mesh != nullptr) {
			for (int j = 0; j < App->modelLoader->allMeshes.size(); ++j) {
				if (App->modelLoader->allMeshes[j]->id == mesh->id) {
					App->modelLoader->allMeshes.erase(App->modelLoader->allMeshes.begin() + j);
				}
			}
			mesh->CleanUp();
		}
		if (shape != nullptr) {
			for (int j = 0; j < App->modelLoader->allShapes.size(); ++j) {
				if (App->modelLoader->allShapes[j]->id == shape->id) {
					App->modelLoader->allShapes.erase(App->modelLoader->allShapes.begin() + j);
				}
			}
			shape->CleanUp();
		}
		//delete parents reference to this
		for (int i = 0; i < parent->meshesOrShapes.size(); ++i) {
			if (parent->meshesOrShapes[i]->id == id) {
				parent->meshesOrShapes.erase(parent->meshesOrShapes.begin() + i);
			}
		}
	}
	for (int i = 0; i < materials.size(); ++i) {
		materials[i]->CleanUp();
		delete materials[i];
	}
	materials.clear();
	
	if (shape != nullptr) {
		for (int i = 0; i < App->modelLoader->allShapes.size(); ++i) {
			if(App->modelLoader->allShapes[i] == shape)
				App->modelLoader->allShapes.erase(App->modelLoader->allShapes.begin() + i);
		}
		shape->CleanUp();
		shape = nullptr;
	}
	if (transform != nullptr) {
		transform->CleanUp();
		delete transform;
	}
	//delete object if is not light
	if (!IsLight()) {
		for (int l = 0; l < App->scene->allObjects.size(); ++l)
			if (App->scene->allObjects[l]->id == id) {
				App->scene->allObjects.erase(App->scene->allObjects.begin() + l);
				return;
			}
	}
	else {
		for (int i = 0; i < App->scene->allLights.size(); ++i)
			if (App->scene->allLights[i]->id == id) App->scene->allLights.erase(App->scene->allLights.begin() + i);
	}
	//delete object if is light
}

void GameObject::deleteChild(unsigned idc) {
	for (int i = 0; i < children.size(); ++i) {
		if (children[i]->id == idc) {
			children.erase(children.begin()+i);
			return;
		}
	}
}

void GameObject::calculateAABB() {
	boundingBox = math::AABB(float3(minX, minY, minZ), float3(maxX, maxY, maxZ));
	boundingBox.TransformAsAABB(transform->transformMatrix);
	updateQuadTree();
}

void GameObject::ChangeName(char* n) {
	name = n;
}

void GameObject::toggleMeshActivation() {
	if (mesh != nullptr) {
		mesh->active = active;
	}
	if (shape != nullptr) {
		shape->active = active;
	}
}

void GameObject::activeToggled() {
	if (Physical) {
		parent->active = !parent->active;
		active = parent->active;
		toggleMeshActivation();
	}
	else {
		for (int i = 0; i < meshesOrShapes.size(); ++i) {
			meshesOrShapes[i]->active = active;
			meshesOrShapes[i]->toggleMeshActivation();
		}
	}
	for (int i = 0; i < materials.size(); ++i) {
		materials[i]->active = active;
	}
	if (hascamera) {
		camera->active = active;
	}
	if(transform != nullptr) transform->active = active;
}

void GameObject::staticToggled(bool first) {
	if(!first)isStatic = !isStatic;
	for (int i = 0; i < children.size(); ++i) {
		children[i]->staticToggled(false);
	}
}

void GameObject::updateQuadTree() {
	for (int i = nodesItAppears.size() - 1; i >= 0 ; --i) {
		nodesItAppears[i]->deleteObject(this);
		nodesItAppears.pop_back();
	}
	App->scene->addIntoQuadTree(this);
}

void GameObject::saveObject(JSON_Value* objValue) {
	JSON_Value* currentValue = objValue->createValue();

	currentValue->addUint("ID", id);
	currentValue->addUint("ParentID", parentId);
	currentValue->addBool("Is active", active);
	currentValue->addBool("Is static", isStatic);
	currentValue->addBool("Bounding box painted", paintBB);
	currentValue->addString("name", name);
	currentValue->addBool("Has camera", hascamera);
	currentValue->addBool("Has mesh", hasmesh);
	currentValue->addBool("Has material", hasmaterial);
	currentValue->addBool("Has shape", hasShape);
	currentValue->addFloat("Min X", minX);
	currentValue->addFloat("Min Y", minY);
	currentValue->addFloat("Min Z", minZ);
	currentValue->addFloat("Max X", maxX);
	currentValue->addFloat("Max Y", maxY);
	currentValue->addFloat("Max Z", maxZ);
	currentValue->addBool("BBgen", BBGenerated);
	currentValue->addBool("Physical", Physical);
	currentValue->addBool("Light", light);

	JSON_Value* Components = objValue->createValue();
	Components->convertToArray();


	//have to create a json value and pass it to every object and component
	//saving components:
	if (hascamera) {
		//when cameras are implemented should do this
		camera->saveCamera(Components);
	}
	if (hasmaterial) {
		for (int i = 0; i < materials.size(); ++i) {
			materials[i]->saveMaterial(Components);
		}
	}
	if (Physical) {
		if (mesh != nullptr) mesh->saveMesh(Components);
		if (shape != nullptr) shape->saveShape(Components);
	}
	else {
		transform->saveTransform(Components);
	}

	currentValue->addValue("Component", Components);

	objValue->addValue("", currentValue);

	//meshes or shapes
	for (int k = 0; k < meshesOrShapes.size(); ++k) meshesOrShapes[k]->saveObject(objValue);
	
	//children
	for (int i = 0; i < children.size(); ++i) children[i]->saveObject(objValue);
}
void GameObject::DrawProperties() {
	ImGui::InputText("Name", (char*)name, 150.0f);
}

void GameObject::DrawShapeEditor() {
	shape->drawEditorMenu();
}

void GameObject::LoadNextMaterial() {
	if (hasmesh) {
		++mesh->mesh.material;
		if (mesh->mesh.material >= parent->materials.size()) mesh->mesh.material = 0;
	}
	else if (hasShape) {
		++shape->material;
		if (shape->material >= parent->materials.size()) shape->material = 0;
	}
}

/*void GameObject::DrawMaterialCreator() {
	if (ImGui::CollapsingHeader("Create manual material")) {
		math::float3 diffuse(0.0f, 0.0f, 0.0f);
		if (ImGui::CollapsingHeader("Diffuse")) {
			ImGui::ColorEdit3("Diffuse color", (float*)&diffuse);
			//DrawComboBoxMaterials("DiffuseComboTextures", MaterialType::DIFFUSE_MAP, diffuseSelected);
			ImGui::Text("Dimensions: %dx%d", 200, 200);
			//ImGui::Image((ImTextureID)material.diffuseMap, ImVec2(200, 200));
			//ImGui::SliderFloat("K diffuse", &material.diffuseK, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Ambient")) {
			DrawComboBoxMaterials("OcclusionComboTextures", MaterialType::OCCLUSION_MAP, occlusionSelected);
			ImGui::Text("Dimensions: %dx%d", material.ambientWidth, material.ambientHeight);
			ImGui::Image((ImTextureID)material.occlusionMap, ImVec2(200, 200));
			ImGui::SliderFloat("K ambient", &material.ambientK, 0.0f, 1.0f);
		}

		if (ImGui::CollapsingHeader("Specular")) {
			ImGui::ColorEdit3("Specular color", (float*)&material.specularColor);
			DrawComboBoxMaterials("SpecularComboTextures", MaterialType::SPECULAR_MAP, specularSelected);
			ImGui::Text("Dimensions: %dx%d", material.specularWidth, material.specularHeight);
			ImGui::Image((ImTextureID)material.specularMap, ImVec2(200, 200));
			ImGui::SliderFloat("K specular", &material.specularK, 0.0f, 1.0f);
			ImGui::SliderFloat("K shininess", &material.shininess, 0.0f, 128.0f);
		}

		if (ImGui::CollapsingHeader("Emissive")) {
			ImGui::ColorEdit3("Emissive color", (float*)&material.emissiveColor);
			DrawComboBoxMaterials("EmissiveComboTextures", MaterialType::EMISSIVE_MAP, emissiveSelected);
			ImGui::Text("Dimensions: %dx%d", material.emissiveWidth, material.emissiveHeight);
			ImGui::Image((ImTextureID)material.emissiveMap, ImVec2(200, 200));
		}

		if (ImGui::Button("save")) {
			ComponentMaterial* mat = new ComponentMaterial(this);
			mat->isTexture = false;

		}
	}
}*/

/*void GameObject::DrawComboBoxMaterials(const char* id, ComponentMaterial mat, static std::string& currentTexture) {
	//std::vector<std::string> fileTexturesList = App->library->fileTexturesList;
	fileTexturesList.insert(fileTexturesList.begin(), "Select a Texture");

	if (fileTexturesList.size() > 0) {
		ImGui::PushID(id);
		if (ImGui::BeginCombo("##", currentTexture.c_str())) {
			for (std::vector<std::string>::iterator iterator = fileTexturesList.begin(); iterator != fileTexturesList.end(); ++iterator) {
				bool isSelected = (currentTexture == (*iterator).c_str());
				if (ImGui::Selectable((*iterator).c_str(), isSelected)) {
					currentTexture = (*iterator).c_str();
					//App->textures->LoadMaterial(currentTexture.c_str(), this, matType);
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}

		ImGui::PopID();
	}
}*/