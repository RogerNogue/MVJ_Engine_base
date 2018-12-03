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
	objects.push_back(baseObject);
	return true;
}

update_status ModuleScene::PreUpdate(){
	return UPDATE_CONTINUE;
}
update_status ModuleScene::Update(){
	return UPDATE_CONTINUE;
}

void ModuleScene::createGameObject(char* c) {
	GameObject newObject(c);
}
void ModuleScene::setUpGameObjectMenu() {
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
		}
		ImGui::EndPopup();
	}

	if (children) {
		for (int i = 0; i < go->children.size(); ++i) {
			paintGameObjectTree(baseObject->children[i]);
		}
		ImGui::TreePop();
	}
	
}