#include "ModuleScene.h"
#include "GameObject.h"
#include "imgui.h"
#include "QuadTreeGnoblin.h"
#include "Serializer.h"

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init() {
	//creating quad tree of the scene
	sceneBoundingBox = AABB(float3(-30, -20, -30), float3(30, 20, 30));
	quadTree = new QuadTreeGnoblin();
	quadTree->Create(sceneBoundingBox);

	baseObject =  new GameObject ("BaseObject");
	//i could also declare the editor camera here
	allObjects.push_back(baseObject);
	return true;
}

void ModuleScene::createGameObject(char* c) {
	GameObject newObject(c);
}
void ModuleScene::setUpGameObjectMenu() {
	if (ImGui::Button("Save scene")) {
		char sceneName[100];
		sprintf_s(sceneName, 100, "Scene_%d.GNOBLIN", sceneNum);
		saveScene(sceneName);
		++sceneNum;
	}
	//maybe drag and drop?
	/*bool loading = true;
	ImGui::Button("Load scene");

	if (loading) {
		if (ImGui::BeginPopupContextItem("load", 1)) {
			loading = false;
			
			for()boost::f
		}
	}else loading = true;*/
	/*if (ImGui::Button("Load scene")) {
		bool loading = true;
		
		if (loading) {

		}
		else loading = true;
	}*/

	bool creating = true;
	ImGui::Checkbox("Draw scene quad tree", &drawQuadTree);
	ImGui::Button("Create");

	if (creating)
	{
		if (ImGui::BeginPopupContextItem("create", 0))
		{
			if (ImGui::Button("Camera"))
			{
				//App->scene_intro->AddCamera();

				creating = false;
			}

			if (ImGui::Button("Empty Object"))
			{
				GameObject* temp = new GameObject("New Object", baseObject);
				creating = false;
			}
			ImGui::EndPopup();
		}
	}else creating = true;
	for (int i = 0; i < baseObject->children.size(); ++i) {
		paintGameObjectTree(baseObject->children[i]);
	}
}
void ModuleScene::paintGameObjectTree(GameObject* go) {

	unsigned int flags;

	flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if(objectSelected != nullptr && objectSelected->id == go->id) flags |= ImGuiTreeNodeFlags_Selected;

	bool children = ImGui::TreeNodeEx(go->name, flags);

	if (ImGui::IsItemClicked(0))
	{
		objectSelected = go;
	}
	if (ImGui::BeginPopupContextItem(go->name, 1))
	{
		if (ImGui::Button("Delete"))
		{
			go->deleteObject();
			objectSelected = nullptr;
		}
		if (ImGui::Button("Create child object"))
		{
			GameObject* temp = new GameObject("New Object", objectSelected);
		}
		ImGui::EndPopup();
	}

	if (children) {
		for (int i = 0; i < go->children.size(); ++i) {
			paintGameObjectTree(go->children[i]);
		}
		ImGui::TreePop();
	}
	
}

void ModuleScene::addIntoQuadTree(GameObject* obj) {
	quadTree->Insert(obj);
}

void ModuleScene::saveScene(const char* name) {
	char fullPath[100];
	strcpy(fullPath, folderPath);
	strcat(fullPath, name);
	JSON_File* json = Serializer::openWriteJSON(fullPath);
	
	//now we go over the object writing in the json
	JSON_Value* objValue = json->createValue();
	objValue->convertToArray();

	baseObject->saveObject(objValue);

	json->addValue("Scene Objects", objValue);
	json->Write();
	json->closeFile();
}
void ModuleScene::loadScene(const char* name) {

}