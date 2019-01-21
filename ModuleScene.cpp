#include "ModuleScene.h"
#include "GameObject.h"
#include "imgui.h"
#include "QuadTreeGnoblin.h"
#include "Serializer.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleCamera.h"

ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init() {
	//creating quad tree of the scene
	sceneBoundingBox = AABB(float3(-300 * sceneScale, -300 * sceneScale, -300 * sceneScale), float3(300 * sceneScale, 300 * sceneScale, 300 * sceneScale));
	quadTree = new QuadTreeGnoblin();
	quadTree->Create(sceneBoundingBox);

	baseObject =  new GameObject ("BaseObject");
	return true;
}

void ModuleScene::createGameObject(const char* c) {
	GameObject newObject(c);
}
void ModuleScene::setUpGameObjectMenu() {
	if (ImGui::Button("Save scene")) {
		char sceneName[100];
		sprintf_s(sceneName, 100, "Scene_%d.GNOBLIN", sceneNum);
		saveScene(sceneName);
		++sceneNum;
	}
	if (ImGui::Button("Load scene")) {
		char sceneName[100];
		sprintf_s(sceneName, 100, "Scene_%d.GNOBLIN", sceneNum);
		loadScene(sceneName);
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
	if (ImGui::Button("Un select object")) objectSelected = nullptr;
	ImGui::Button("Create");
	if (creating)
	{
		
		if (ImGui::BeginPopupContextItem("create", 0))
		{
			if (ImGui::Button("Camera"))
			{

				creating = false;
			}
			if (ImGui::Button("Light"))
			{
				GameObject* obj = new GameObject("Light source", baseObject, 1, true);
				allLights.push_back(obj);
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

	ImGui::PushID(go->id);
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
	ImGui::PopID();
	if (children) {
		for (int i = 0; i < go->meshesOrShapes.size(); ++i) {
			paintGameObjectTree(go->meshesOrShapes[i]);
		}
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

	char* b = new char[100];
	sprintf(b, "Saving Scene: ");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog(name);
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;

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
	char* b = new char[100];
	sprintf(b, "Loading Scene: ");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog("Scene_1.GNOBLIN");
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;

	//delete current scene
	baseObject->deleteObject();

	//load new scene
	char fullPath[100];
	strcpy(fullPath, folderPath);
	strcat(fullPath, "Scene_1.GNOBLIN");
	JSON_File* json = Serializer::openReadJSON(fullPath);
	if (json == nullptr) {
		char* b = new char[100];
		sprintf(b, "Scene with name ");
		App->menu->console.AddLog(b);
		App->menu->console.AddLog(name);
		sprintf(b, "Does not exist.\n\n");
		App->menu->console.AddLog(b);
		delete[] b;
		return;
	}
	JSON_Value* sceneObjects = json->getValue("Scene Objects");
	if (sceneObjects->getRapidJSONValue()->IsArray()) {
		std::vector<GameObject*> redObjects;
		for (int i = 0; i < sceneObjects->getRapidJSONValue()->Size(); ++i) {
			GameObject* obj = new GameObject(sceneObjects->getValueFromArray(i));
			redObjects.push_back(obj);
		}
		for (int i = 0; i < redObjects.size(); ++i) {
			if (redObjects[i]->parentId != 0 && redObjects[i]->parent == nullptr) {
				//if it has a parent, we look for him like the good people we are
				for (int j = 0; j < redObjects.size(); ++j) {
					if (redObjects[j]->id == redObjects[i]->parentId) {
						redObjects[i]->parent = redObjects[j];
						if (redObjects[i]->isPhysical()) {
							redObjects[j]->meshesOrShapes.push_back(redObjects[i]);
						}
						else {
							redObjects[j]->children.push_back(redObjects[i]);
						}
					}
				}
			}
			else {
				baseObject = redObjects[i];
			}
		}
		//clear the pointer since it's not gonna be used again
		//for (int i = 0; i < redObjects.size(); ++i) delete redObjects[i];
	}
	json->closeFile();
}

void ModuleScene::mouseClick(int mouseXi, int mouseYi) {
	float mouseX = (float)mouseXi;
	float mouseY = (float)mouseYi;
	math::float2 viewportTopLeft(App->menu->menubarWidth, 50.0f);
	math::float2 viewportSize(App->camera->screenWidth - 2 * App->menu->menubarWidth, App->camera->screenHeight - App->menu->menubarHeight - 20);
	math::float2 windowSize(App->camera->screenWidth, App->camera->screenHeight);
	float consoleH = App->menu->menubarHeight;
	float endpointX = App->menu->menubarWidth + App->camera->screenWidth - 2 * App->menu->menubarWidth;
	float endpointY = 20.0f + App->camera->screenHeight - App->menu->menubarHeight - 20;
	if (mouseX > App->menu->menubarWidth && mouseX < (endpointX) &&
		mouseY > 20.0f && mouseY < endpointY) {
		float sy, sx, ty, tx;
		/*sy = (1 + 1) / (-viewportSize.y);
		sx = (1 + 1) / (viewportSize.x);
		ty = (-(viewportSize.y + viewportTopLeft.y) - viewportTopLeft.y) / (-viewportSize.y);
		tx = (-(viewportSize.x + viewportTopLeft.x) - viewportTopLeft.x) / (viewportSize.x);*/

		sy = (1 + 1) / (-windowSize.y);
		sx = (1 + 1) / (windowSize.x);
		ty = (-(viewportTopLeft.y) - (viewportTopLeft.y + windowSize.y)) / (-windowSize.y);
		tx = (-(windowSize.x + viewportTopLeft.x) - viewportTopLeft.x) / (windowSize.x);

		float normX = sx*(mouseX) + tx - 0.01;
		float normY = sy*(mouseY) + ty;
		ray = App->camera->frustum.UnProjectLineSegment(normX, normY);

		//future implementation: make quadtree work fully and use it to make this algorithm more efficient
		//now we check collisions
		std::vector<GameObject*> collisions;
		for (int i = 0; i < allObjects.size(); ++i) {
			if (ray.Intersects(allObjects[i]->boundingBox)) {
				collisions.push_back(allObjects[i]);
			}
		}
		GameObject* picked = nullptr;
		if(collisions.size() > 0) picked = collisions[0];
		for (int i = 0; i < collisions.size(); ++i) {
			picked = closestToCam(picked, collisions[i]);
		}
		//now that we have the picked object, make it selected
		if (picked != nullptr) {
			objectSelected = picked;
		}
	}
}

GameObject* ModuleScene::closestToCam(GameObject* go1, GameObject* go2) {
	if (go1->boundingBox.Distance(App->camera->frustum.pos) < go2->boundingBox.Distance(App->camera->frustum.pos)) return go1;
	else return go2;
}