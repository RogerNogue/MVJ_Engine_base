#include "ModuleScene.h"
#include "GameObject.h"

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