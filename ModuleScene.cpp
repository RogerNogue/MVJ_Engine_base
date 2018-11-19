#include "ModuleScene.h"
#include "GameObject.h"



ModuleScene::ModuleScene()
{
}


ModuleScene::~ModuleScene()
{
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