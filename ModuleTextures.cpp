#include "Globals.h"
#include "ModuleTextures.h"

#include <GL/glew.h>

#include <IL/il.h>
#include <IL/ilut.h>
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "Brofiler.h"

ModuleTextures::ModuleTextures()
{
}

// Destructor
ModuleTextures::~ModuleTextures()
{
}

// Called before render is available
bool ModuleTextures::Init()
{
	ilInit();
	iluInit();
	ilutInit();

	return true;
}

// Load new texture from file path
unsigned ModuleTextures::Load(const char* path, bool mipmaps, float* width, float* height)
{
	BROFILER_CATEGORY("Load texture", Profiler::Color::Cyan)
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	if (ilLoadImage(path))
	{
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		int channels = ilGetInteger(IL_IMAGE_CHANNELS);
		if (channels == 3)
		{
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}
		else if (channels == 4)
		{
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		}
		*width = ilGetInteger(IL_IMAGE_WIDTH);
		*height = ilGetInteger(IL_IMAGE_HEIGHT);
		ILubyte* data = ilGetData();
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		ilDeleteImages(1, &imageId);

		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}

	return 0;
}

void ModuleTextures::Unload(unsigned id)
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
	}
}


ComponentMaterial ModuleTextures::createComponentMaterial(GameObject* dad) {
	ComponentMaterial c(dad);
	return c;
}

unsigned ModuleTextures::createFrameBuffer(char* name, float width, float height) {
	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	/*ILubyte* data = ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/
	return textureId;
}

void ModuleTextures::deleteFrameBuffer(unsigned buffer) {
	ilDeleteImages(1, &buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleTextures::CreateMaterial(std::string path, unsigned& textureID, int& width, int& height) {
	unsigned imageID;

	ilGenImages(1, &imageID);

	ilBindImage(imageID);

	path.insert(0, "/Library/Textures/");

	LOG("Loading material %s", path.c_str());

	char* fileBuffer;
	//unsigned lenghBuffer = App->fileSystem->Load(path.c_str(), &fileBuffer);
	unsigned lenghBuffer = 0;
	if (ilLoadL(IL_DDS, fileBuffer, lenghBuffer)) {
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

		if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE)) {
			LOG("Error: Image conversion failed %s", iluErrorString(ilGetError()));
			return;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	}

	ilDeleteImages(1, &imageID);
	LOG("Material creation successful.");
}