#include "ModuleScene.h"
#include "GameObject.h"
#include "imgui.h"

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init() {
	baseObject =  new GameObject ("BaseObject");
	//i could also declare the editor camera here
	baseObjects.push_back(baseObject);
	return true;
}

void ModuleScene::createGameObject(char* c) {
	GameObject newObject(c);
}
void ModuleScene::setUpGameObjectMenu() {
	bool creating = true;
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