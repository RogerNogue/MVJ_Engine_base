#include "ModuleModelLoader.h"
#include "Application.h"
#include "ModuleMenu.h"
#include "ModuleProgram.h"
#include "ModuleTextures.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentShape.h"
#include "ComponentMaterial.h"
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "ModuleScene.h"
#include "GameObject.h"
#include "Brofiler.h"

#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"

ModuleModelLoader::ModuleModelLoader()
{

}


ModuleModelLoader::~ModuleModelLoader()
{
}

void ModuleModelLoader::deleteVBO(unsigned vbo) {
	glDeleteBuffers(1, &vbo);
}

void ModuleModelLoader::deleteVIO(unsigned vio) {
	glDeleteBuffers(1, &vio);
}

void ModuleModelLoader::unloadTexture(unsigned tex0) {
	App->textures->Unload(tex0);
}

void ModuleModelLoader::loadModel(unsigned model, GameObject* object) {
	BROFILER_CATEGORY("Model loading", Profiler::Color::Lime)
	const aiScene* scene;
	currentModel = model;
	currentModelTriangleCount = 0;
	const char* modelName;
	if (model == 1) {
		scene = aiImportFile("Assets/models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Baker House";
	}
	else if (model == 2) {
		scene = aiImportFile("Assets/models/banana/banana.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Banana";
	}
	else if (model == 3) {
		scene = aiImportFile("Assets/models/shield/Shield.FBX", aiProcessPreset_TargetRealtime_MaxQuality);
		modelName = "Shield";
	}
	if (scene == nullptr) {
		LOG(aiGetErrorString());
	}
	char* b = new char[100];
	sprintf(b, "Loading new model:\n");
	App->menu->console.AddLog(b);
	App->menu->console.AddLog(modelName);
	if (scene->mNumMeshes <= 0) {
		sprintf(b, "The model loaded has no meshes:\n");
		App->menu->console.AddLog(b);
		sprintf(b, "\n\n");
		App->menu->console.AddLog(b);
		delete[] b;
		return;
	}
	sprintf(b, "\n\n");
	App->menu->console.AddLog(b);
	delete[] b;
	//game object creation and fill of its mesh and material components
	if (object == nullptr) {
		object = new GameObject(modelName, App->scene->baseObject);
		object->minX = object->maxX = scene->mMeshes[0]->mVertices[0].x;
		object->minY = object->maxY = scene->mMeshes[0]->mVertices[0].y;
		object->minZ = object->maxZ = scene->mMeshes[0]->mVertices[0].z;
	}

	for (unsigned i = 0; i < scene->mNumMeshes; ++i)
	{
		GenerateMeshData(scene->mMeshes[i], object, i, model);
		currentModelTriangleCount += scene->mMeshes[i]->mNumVertices / 3;
	}
	object->calculateAABB();
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		GenerateMaterialData(scene->mMaterials[i], object, model, i);
	}
	aiReleaseImport(scene);
	App->camera->mewModelLoaded();
}

void ModuleModelLoader::unloadModels() {
	CleanUp();
}

void ModuleModelLoader::GenerateMeshData(const aiMesh* mesh, GameObject* Obj, int numMesh, int numModel) {
	GameObject* meshObject = new GameObject("MeshObject", Obj, true);
	ComponentMesh* newMesh = new ComponentMesh(meshObject);
	newMesh->numMesh = numMesh;
	newMesh->numModel = numModel;
	//vbo
	glGenBuffers(1, &newMesh->mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->mesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*mesh->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->mNumVertices * 3 * sizeof(float), mesh->mVertices);
	
	//buffer for the faces (vio)
	math::float2* textureCoords = (math::float2*) glMapBufferRange(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(float), mesh->mNumVertices * 2 * sizeof(float), GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
		textureCoords[i] = math::float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		//i use this loop to update the bounding box values
		if (Obj->minX > mesh->mVertices[i].x) {
			Obj->minX = mesh->mVertices[i].x;
		}
		else if (Obj->maxX < mesh->mVertices[i].x) {
			Obj->maxX = mesh->mVertices[i].x;
		}
		if (Obj->minY > mesh->mVertices[i].y) {
			Obj->minY = mesh->mVertices[i].y;
		}
		else if (Obj->maxY < mesh->mVertices[i].y) {
			Obj->maxY = mesh->mVertices[i].y;
		}
		if (Obj->minZ > mesh->mVertices[i].z) {
			Obj->minZ = mesh->mVertices[i].z;
		}
		else if (Obj->maxZ < mesh->mVertices[i].z) {
			Obj->maxZ = mesh->mVertices[i].z;
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &newMesh->mesh.vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->mesh.vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*mesh->mNumFaces * 3, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
													sizeof(unsigned)*mesh->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);

		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	newMesh->mesh.material = mesh->mMaterialIndex;
	newMesh->mesh.numIndices = mesh->mNumFaces*3;
	newMesh->mesh.numVertices = mesh->mNumVertices;
	meshObject->mesh = newMesh;
	meshObject->hasmesh = true;
	allMeshes.push_back(newMesh);
}

void ModuleModelLoader::GenerateMaterialData(const aiMaterial* mat, GameObject* Obj, int model, int i) {
	ComponentMaterial* newMat = new ComponentMaterial(Obj);

	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;
	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS) {
		newMat->material.texture0 = App->textures->Load(file.data, false, &newMat->material.sizeX, &newMat->material.sizeY);
	}
	if (currentModel == 2) newMat->material.texture0 = App->textures->Load("Assets/models/banana/banana.png", false, &newMat->material.sizeX, &newMat->material.sizeY);
	if(currentModel == 3) newMat->material.texture0 = App->textures->Load("Assets/models/shield/tex.png", false, &newMat->material.sizeX, &newMat->material.sizeY);
	
	Obj->materials.push_back(newMat);
	Obj->materials[Obj->materials.size() - 1]->numModel = model;
	Obj->materials[Obj->materials.size() - 1]->numMaterial = i;
	Obj->hasmaterial = true;
}


bool ModuleModelLoader::CreateSphere(GameObject* object) {
	GameObject* shapeObject = new GameObject("ShapeObject", object, true);
	ComponentShape* newShape = new ComponentShape(shapeObject, SPHERE);
	
	ComponentMaterial* mat = new ComponentMaterial(newShape->dad->parent);
	newShape->material = newShape->dad->parent->materials.size();
	newShape->dad->parent->materials.push_back(mat);

	LoadSphere(newShape, true);
	return true;
}
bool ModuleModelLoader::CreateCube(GameObject* object) {
	GameObject* shapeObject = new GameObject("ShapeObject", object, true);
	ComponentShape* newShape = new ComponentShape(shapeObject, CUBE);

	ComponentMaterial* mat = new ComponentMaterial(newShape->dad->parent);
	newShape->material = newShape->dad->parent->materials.size();
	newShape->dad->parent->materials.push_back(mat);

	LoadCube(newShape, true);
	return true;
}
bool ModuleModelLoader::CreateCylinder(GameObject* object) {
	GameObject* shapeObject = new GameObject("ShapeObject", object, true);
	ComponentShape* newShape = new ComponentShape(shapeObject, CYLINDER);

	ComponentMaterial* mat = new ComponentMaterial(newShape->dad->parent);
	newShape->material = newShape->dad->parent->materials.size();
	newShape->dad->parent->materials.push_back(mat);

	LoadCylinder(newShape, true);
	return true;
}
bool ModuleModelLoader::CreateTorus(GameObject* object) {
	GameObject* shapeObject = new GameObject("ShapeObject", object, true);
	ComponentShape* newShape = new ComponentShape(shapeObject, TORUS);

	ComponentMaterial* mat = new ComponentMaterial(newShape->dad->parent);
	newShape->material = newShape->dad->parent->materials.size();
	newShape->dad->parent->materials.push_back(mat);

	LoadTorus(newShape, true);
	return true;
}

bool ModuleModelLoader::LoadSphere(ComponentShape* sphere, bool newSphere) {

	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(sphere->slices, sphere->stacks);

	if (mesh)
	{
		par_shapes_scale(mesh, sphere->size1, sphere->size1, sphere->size1);

		generateShape(mesh, sphere, sphere->dad->parent->materials[sphere->material], newSphere);
		par_shapes_free_mesh(mesh);

		return true;
	}
	return false;
}
bool ModuleModelLoader::LoadCube(ComponentShape* cube, bool newCube) {
	par_shapes_mesh* mesh = par_shapes_create_plane(1, 1);
	par_shapes_mesh* top = par_shapes_create_plane(1, 1);
	par_shapes_mesh* bottom = par_shapes_create_plane(1, 1);
	par_shapes_mesh* back = par_shapes_create_plane(1, 1);
	par_shapes_mesh* left = par_shapes_create_plane(1, 1);
	par_shapes_mesh* right = par_shapes_create_plane(1, 1);

	par_shapes_translate(mesh, -0.5f, -0.5f, 0.5f);

	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, -0.5f, 0.5f, 0.5f);

	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(back, float(PAR_PI), (float*)&math::float3::unitX);
	par_shapes_translate(back, -0.5f, 0.5f, -0.5f);

	par_shapes_rotate(left, float(-PAR_PI * 0.5), (float*)&math::float3::unitY);
	par_shapes_translate(left, -0.5f, -0.5f, -0.5f);

	par_shapes_rotate(right, float(PAR_PI*0.5), (float*)&math::float3::unitY);
	par_shapes_translate(right, 0.5f, -0.5f, 0.5f);

	par_shapes_merge_and_free(mesh, top);
	par_shapes_merge_and_free(mesh, bottom);
	par_shapes_merge_and_free(mesh, back);
	par_shapes_merge_and_free(mesh, left);
	par_shapes_merge_and_free(mesh, right);

	if (mesh)
	{
		par_shapes_scale(mesh, cube->size1, cube->size2, cube->size3);
		generateShape(mesh, cube, cube->dad->parent->materials[cube->material], newCube);
		par_shapes_free_mesh(mesh);

		return true;
	}

	return false;
}
bool ModuleModelLoader::LoadCylinder(ComponentShape* cylinder, bool newCylinder) {
	//cylinder size1 = radius, size2 = height
	par_shapes_mesh* mesh = par_shapes_create_cylinder(int(cylinder->slices), int(cylinder->stacks));
	par_shapes_rotate(mesh, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(mesh, 0.0f, -0.5f, 0.0f);

	par_shapes_mesh* top = par_shapes_create_disk(cylinder->size1, int(cylinder->slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(top, -float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(top, 0.0f, cylinder->size2*0.5f, 0.0f);

	par_shapes_mesh* bottom = par_shapes_create_disk(cylinder->size1, int(cylinder->slices), (const float*)&math::float3::zero, (const float*)&math::float3::unitZ);
	par_shapes_rotate(bottom, float(PAR_PI*0.5), (float*)&math::float3::unitX);
	par_shapes_translate(bottom, 0.0f, cylinder->size2*-0.5f, 0.0f);

	if (mesh)
	{
		par_shapes_scale(mesh, cylinder->size1, cylinder->size2, cylinder->size1);
		par_shapes_merge_and_free(mesh, top);
		par_shapes_merge_and_free(mesh, bottom);

		generateShape(mesh, cylinder, cylinder->dad->parent->materials[cylinder->material], newCylinder);
		par_shapes_free_mesh(mesh);

		return true;
	}

	return false;
}
bool ModuleModelLoader::LoadTorus(ComponentShape* torus, bool newTorus) {
	//size1 = inner radius, size2 = outer radius
	par_shapes_mesh* mesh = par_shapes_create_torus(torus->slices, torus->stacks, torus->size1);

	if (mesh)
	{
		par_shapes_scale(mesh, torus->size2, torus->size2, torus->size2);
		generateShape(mesh, torus, torus->dad->parent->materials[torus->material], newTorus);
		par_shapes_free_mesh(mesh);

		return true;
	}

	return false;
}

void ModuleModelLoader::generateShape(par_shapes_mesh* shape, ComponentShape* comp, ComponentMaterial* mat, bool isNew) {
	glGenBuffers(1, &comp->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, comp->vbo);

	// bounding box calculation
	for (unsigned i = 0; i< unsigned(shape->npoints); ++i)
	{
		math::float3 point(shape->points[i * 3], shape->points[i * 3 + 1], shape->points[i * 3 + 2]);
		
		comp->dad->parent->minX = min(comp->dad->parent->minX, point[0]);
		comp->dad->parent->maxX = min(comp->dad->parent->maxX, point[0]);
		comp->dad->parent->minY = min(comp->dad->parent->minY, point[1]);
		comp->dad->parent->maxY = min(comp->dad->parent->maxY, point[1]);
		comp->dad->parent->minZ = min(comp->dad->parent->minZ, point[2]);
		comp->dad->parent->maxZ = min(comp->dad->parent->maxZ, point[2]);
	}

	unsigned offset_acc = sizeof(math::float3);

	if (shape->normals)
	{
		comp->normals_offset = offset_acc;
		offset_acc += sizeof(math::float3);
	}

	comp->numVertices = offset_acc;

	glBufferData(GL_ARRAY_BUFFER, comp->numVertices*shape->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3)*shape->npoints, shape->points);

	// normals

	if (shape->normals)
	{
		glBufferSubData(GL_ARRAY_BUFFER, comp->normals_offset*shape->npoints, sizeof(math::float3)*shape->npoints, shape->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// indices

	glGenBuffers(1, &comp->vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp->vio);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*shape->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*shape->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(shape->ntriangles * 3); ++i)
	{
		*(indices++) = shape->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	comp->material = 0;
	comp->numVertices = shape->npoints;
	comp->numIndices = shape->ntriangles * 3;


	//GenerateVAO(dst_mesh);
	if(isNew) allShapes.push_back(comp);

	/*bsphere.center = (max_v + min_v)*0.5f;
	bsphere.radius = (max_v - min_v).Length()*0.5f;*/
}

void ModuleModelLoader::GenerateOneMeshData(ComponentMesh* newMesh) {
	const aiScene* scene = aiImportFile("models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	if (newMesh->numModel == 1) {
		scene = aiImportFile("Assets/models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	}
	else if (newMesh->numModel == 2) {
		scene = aiImportFile("Assets/models/banana/banana.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	}
	else if (newMesh->numModel == 3) {
		scene = aiImportFile("Assets/models/shield/Shield.FBX", aiProcessPreset_TargetRealtime_MaxQuality);
	}

	//vbo
	glGenBuffers(1, &newMesh->mesh.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh->mesh.vbo);

	glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 3 + sizeof(float) * 2)*scene->mMeshes[newMesh->numMesh]->mNumVertices, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, scene->mMeshes[newMesh->numMesh]->mNumVertices * 3 * sizeof(float), scene->mMeshes[newMesh->numMesh]->mVertices);

	//buffer for the faces (vio)
	math::float2* textureCoords = (math::float2*) glMapBufferRange(GL_ARRAY_BUFFER, 
		scene->mMeshes[newMesh->numMesh]->mNumVertices * 3 * sizeof(float),
		scene->mMeshes[newMesh->numMesh]->mNumVertices * 2 * sizeof(float), GL_MAP_WRITE_BIT);
	for (unsigned i = 0; i < scene->mMeshes[newMesh->numMesh]->mNumVertices; ++i) {
		textureCoords[i] = math::float2(scene->mMeshes[newMesh->numMesh]->mTextureCoords[0][i].x, scene->mMeshes[newMesh->numMesh]->mTextureCoords[0][i].y);
		//i use this loop to update the bounding box values
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &newMesh->mesh.vio);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh->mesh.vio);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*scene->mMeshes[newMesh->numMesh]->mNumFaces * 3, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*scene->mMeshes[newMesh->numMesh]->mNumFaces * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i < scene->mMeshes[newMesh->numMesh]->mNumFaces; ++i)
	{
		assert(scene->mMeshes[newMesh->numMesh]->mFaces[i].mNumIndices == 3);

		*(indices++) = scene->mMeshes[newMesh->numMesh]->mFaces[i].mIndices[0];
		*(indices++) = scene->mMeshes[newMesh->numMesh]->mFaces[i].mIndices[1];
		*(indices++) = scene->mMeshes[newMesh->numMesh]->mFaces[i].mIndices[2];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	newMesh->mesh.material = scene->mMeshes[newMesh->numMesh]->mMaterialIndex;
	newMesh->mesh.numIndices = scene->mMeshes[newMesh->numMesh]->mNumFaces * 3;
	newMesh->mesh.numVertices = scene->mMeshes[newMesh->numMesh]->mNumVertices;
	allMeshes.push_back(newMesh);
}

void ModuleModelLoader::GenerateOneMaterialData(ComponentMaterial* newMaterial) {
	const aiScene* scene = aiImportFile("models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	if (newMaterial->numModel == 1) {
		scene = aiImportFile("Assets/models/baker_house/BakerHouse.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	}
	else if (newMaterial->numModel == 2) {
		scene = aiImportFile("Assets/models/banana/banana.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
	}
	else if (newMaterial->numModel == 3) {
		scene = aiImportFile("Assets/models/shield/Shield.FBX", aiProcessPreset_TargetRealtime_MaxQuality);
	}
	aiString file;
	aiTextureMapping mapping;
	unsigned uvindex = 0;
	if (scene->mMaterials[newMaterial->numMaterial]->GetTexture(aiTextureType_DIFFUSE, 0, &file, &mapping, &uvindex) == AI_SUCCESS) {
		newMaterial->material.texture0 = App->textures->Load(file.data, false, &newMaterial->material.sizeX, &newMaterial->material.sizeY);
	}
	if (newMaterial->numModel == 2) newMaterial->material.texture0 = App->textures->Load("Assets/models/banana/banana.png", false, &newMaterial->material.sizeX, &newMaterial->material.sizeY);
	if (newMaterial->numModel == 3) newMaterial->material.texture0 = App->textures->Load("Assets/models/shield/tex.png", false, &newMaterial->material.sizeX, &newMaterial->material.sizeY);

}