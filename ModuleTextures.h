#ifndef __ModuleTextures_H__
#define __ModuleTextures_H__

#include<list>
#include "Module.h"

class GameObject;
class ComponentMaterial;

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();
	bool CleanUp();

	unsigned Load(const char* path, bool mipmaps);
	void Unload(unsigned id);
	ComponentMaterial createComponentMaterial(GameObject* dad);
	unsigned createFrameBuffer(char* name, float width, float height);
	void deleteFrameBuffer(unsigned buffer);

private:
};

#endif