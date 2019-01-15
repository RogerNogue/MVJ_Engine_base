#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "ModuleModelLoader.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "debugdraw.h"
#include "Brofiler.h"
#include "ModuleDebugDraw.h"
#include "imgui.h"
#include "ComponentTransform.h"
#include "ModuleScene.h"
#include "QuadTreeGnoblin.h"
#include "ComponentShape.h"
#include "ModuleMenu.h"
#include "ImGuizmo.h"

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	context = SDL_GL_CreateContext(App->window->window);

	glewInit();

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_2D);

	glClearDepth(1.0f);
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);

    int width, height;
    SDL_GetWindowSize(App->window->window, &width, &height);
    glViewport(0, 0, width, height);

	App->modelLoader->loadModel(1, nullptr);
	UpdateEditorCamera();
	
	return true;
}

update_status ModuleRender::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}
void ModuleRender::UpdateEditorCamera() {	

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, App->camera->screenWidth, App->camera->screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ModuleRender::RenderWithNormals() {

}

void ModuleRender::RenderWithNoNormals() {

}

void ModuleRender::RenderMeshes() {
	//drawing the model
	for (unsigned j = 0; j < App->modelLoader->allMeshes.size(); ++j) {
		if (App->modelLoader->allMeshes[j]->active) {
			GLuint* currentprog = nullptr;
			if (App->modelLoader->allMeshes[j]->mesh.normalsOffset != 0) {
				ComponentMaterial* currentMat = App->modelLoader->allMeshes[j]->dad->parent->materials[App->modelLoader->allMeshes[j]->mesh.material];
				if (currentMat->isTexture) {
					currentprog = &App->shaderProgram->programBlinnPhongTexture;
					glUseProgram(*currentprog);
				}
				else {
					currentprog = &App->shaderProgram->programBlinnPhong;
					glUseProgram(*currentprog);
				}

				glUniform4f(glGetUniformLocation(*currentprog,
					"object_color"), currentMat->surface.difuseColor.x, currentMat->surface.difuseColor.y,
					currentMat->surface.difuseColor.z, currentMat->surface.difuseColor.w);
				const int size = App->scene->allLights.size() * 3;
				float* arrayLights = (float*)malloc(sizeof(float)*App->scene->allLights.size() * 3);
				//now we fill an array of lights to pass the shader
				for (int i = 0; i < App->scene->allLights.size(); ++i) {
					arrayLights[sizeof(float)*i * 3] = App->scene->allLights[i]->transform->positionValues.x;
					arrayLights[sizeof(float)*i * 3 + 1] = App->scene->allLights[i]->transform->positionValues.y;
					arrayLights[sizeof(float)*i * 3 + 2] = App->scene->allLights[i]->transform->positionValues.z;
				}

				glUniform3f(glGetUniformLocation(*currentprog,
					"light_pos"), arrayLights[0], arrayLights[1], arrayLights[2]);

				glUniform1f(glGetUniformLocation(*currentprog,
					"ambient"), App->modelLoader->allMeshes[j]->ambient);
				glUniform1f(glGetUniformLocation(*currentprog,
					"shininess"), currentMat->shininess);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_ambient"), currentMat->k_ambient);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_diffuse"), currentMat->k_diffuse);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_specular"), currentMat->k_specular);
				delete arrayLights;

			}else currentprog = &App->shaderProgram->programTexture;
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"model"), 1, GL_TRUE, &App->modelLoader->allMeshes[j]->dad->parent->transform->transformMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"view"), 1, GL_TRUE, &App->camera->view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

			glActiveTexture(GL_TEXTURE0);
			ComponentMaterial* temp = App->modelLoader->allMeshes[j]->dad->parent->materials[App->modelLoader->allMeshes[j]->mesh.material];
			if (temp->active) glBindTexture(GL_TEXTURE_2D, temp->material.texture0);
			glUniform1i(glGetUniformLocation(*currentprog, "texture0"), 0);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, App->modelLoader->allMeshes[j]->mesh.vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(App->modelLoader->allMeshes[j]->mesh.numVertices * App->modelLoader->allMeshes[j]->mesh.texCoordsOffset));
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(App->modelLoader->allMeshes[j]->mesh.normalsOffset*App->modelLoader->allMeshes[j]->mesh.numVertices));

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->allMeshes[j]->mesh.vio);

			glDrawElements(GL_TRIANGLES, App->modelLoader->allMeshes[j]->mesh.numIndices, GL_UNSIGNED_INT, nullptr);


			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	}
}

void ModuleRender::RenderShapes() {
	for (unsigned j = 0; j < App->modelLoader->allShapes.size(); ++j) {
		ComponentMaterial* currentMat = App->modelLoader->allShapes[j]->dad->parent->materials[App->modelLoader->allShapes[j]->material];
		if (App->modelLoader->allShapes[j]->active) {
			GLuint* currentprog = nullptr;
			if (App->modelLoader->allShapes[j]->normals_offset != 0) {
				if (currentMat->isTexture) {
					currentprog = &App->shaderProgram->programBlinnPhongTexture;
				}
				else {
					currentprog = &App->shaderProgram->programBlinnPhong;
					glUseProgram(*currentprog);
				}
			}
			else currentprog = &App->shaderProgram->programTexture;
			glUseProgram(*currentprog);
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"model"), 1, GL_TRUE, &App->modelLoader->allShapes[j]->dad->parent->transform->transformMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"view"), 1, GL_TRUE, &App->camera->view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(*currentprog,
				"proj"), 1, GL_TRUE, &App->camera->projection[0][0]);

			glActiveTexture(GL_TEXTURE0);

			if (currentMat->active) glBindTexture(GL_TEXTURE_2D, currentMat->material.texture0);
			glUniform1i(glGetUniformLocation(*currentprog, "texture0"), 0);

			glEnableVertexAttribArray(0);
			
			glBindBuffer(GL_ARRAY_BUFFER, App->modelLoader->allShapes[j]->vbo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			

			if (App->modelLoader->allShapes[j]->normals_offset != 0) {
				glUniform4f(glGetUniformLocation(*currentprog,
					"object_color"),	currentMat->surface.difuseColor.x, currentMat->surface.difuseColor.y,
								currentMat->surface.difuseColor.z, currentMat->surface.difuseColor.w);
				const int size = App->scene->allLights.size() * 3;
				float* arrayLights = (float*)malloc(sizeof(float)*App->scene->allLights.size() *3);
				//now we fill an array of lights to pass the shader
				for (int i = 0; i < App->scene->allLights.size(); ++i) {
					arrayLights[sizeof(float)*i * 3] = App->scene->allLights[i]->transform->positionValues.x;
					arrayLights[sizeof(float)*i * 3 + 1] = App->scene->allLights[i]->transform->positionValues.y;
					arrayLights[sizeof(float)*i * 3 + 2] = App->scene->allLights[i]->transform->positionValues.z;
				}
				/*glUniform3fv(glGetUniformLocation(*currentprog,
					"light_pos"), 1, arrayLights[0]);*/
				glUniform3f(glGetUniformLocation(*currentprog,
					"light_pos"), arrayLights[0], arrayLights[1], arrayLights[2]);

				glUniform1f(glGetUniformLocation(*currentprog,
					"ambient"),  App->modelLoader->allShapes[j]->ambient);
				glUniform1f(glGetUniformLocation(*currentprog,
					"shininess"), currentMat->shininess);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_ambient"), currentMat->k_ambient);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_diffuse"), currentMat->k_diffuse);
				glUniform1f(glGetUniformLocation(*currentprog,
					"k_specular"), currentMat->k_specular);
				delete arrayLights;

				glEnableVertexAttribArray(2);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(App->modelLoader->allShapes[j]->numVertices * App->modelLoader->allShapes[j]->normals_offset));
				
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->modelLoader->allShapes[j]->vio);

			glDrawElements(GL_TRIANGLES, App->modelLoader->allShapes[j]->numIndices, GL_UNSIGNED_INT, nullptr);


			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(2);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}
	}
}

// Called every draw update
void ModuleRender::Draw()
{
	BROFILER_CATEGORY("RendererDrawFunction", Profiler::Color::Chartreuse)

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, App->camera->screenWidth, App->camera->screenHeight);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderMeshes();

	RenderShapes();

	dd::xzSquareGrid(-40.0f, 40.0f, 0.0f, 1.0f, math::float3(0.65f, 0.65f, 0.65f));
	dd::axisTriad(math::float4x4::identity, 5*0.125f, 5*1.25f, 0, false);
	//loop to paint all the bounding boxes
	for (int i = 0; i < App->scene->allObjects.size(); ++i) {
		if (App->scene->allObjects[i]->paintBB) {
			dd::aabb(App->scene->allObjects[i]->boundingBox.minPoint, App->scene->allObjects[i]->boundingBox.maxPoint, float3(0.4f,0.8f,0.2f));
		}
	}
	//draw ray from mouse picking
	if (App->scene->drawSegment) {
		for (int i = 0; i < 10; ++i) {
			for (int j = 0; j < 10; ++j) {
				float newStartX = App->scene->ray.a.x +0.001*i;
				float newStartY = App->scene->ray.a.y +0.001*j;
				float newEndX = App->scene->ray.b.x +0.001*i;
				float newEndY = App->scene->ray.b.y +0.001*j;
				math:float3 newStart(newStartX, newStartY, App->scene->ray.a.z);
				math::float3 newEnd(newEndX, newEndY, App->scene->ray.b.z);

				dd::line(newStart, newEnd, math::float3(0.8, 0.3, 0.3));
			}
		}
	}
	//check if quad tree is to be painted
	if (App->scene->drawQuadTree) {
		App->scene->quadTree->DrawQuadTree();
	}

	App->debugDraw->Draw(App->camera, frameBuffer, App->camera->screenWidth, App->camera->screenHeight);

	//draw gizmo
	if (App->scene->objectSelected != nullptr) {
		drawGizmo();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

update_status ModuleRender::PostUpdate()
{
	App->menu->DrawMenu();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	SDL_GL_DeleteContext(App->window->window);
	context = nullptr;

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
    glViewport(0, 0, width, height);
	App->camera->SetAspectRatio(width, height);
	UpdateEditorCamera();
}

ComponentMesh ModuleRender::createComponentMesh(GameObject* dad) {
	ComponentMesh c(dad);
	return c;
}

void ModuleRender::setUpViewport() {
	//generating texture
	if (texWidth != App->camera->screenWidth || texHeight != App->camera->screenHeight) {

		texWidth = App->camera->screenWidth;
		texHeight = App->camera->screenHeight;
		if (renderTexture != 0)
		{
			glDeleteTextures(1, &renderTexture);
		}

		if (texWidth != 0 && texHeight != 0)
		{
			if (frameBuffer == 0)
			{
				glGenFramebuffers(1, &frameBuffer);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			glGenTextures(1, &renderTexture);
			glBindTexture(GL_TEXTURE_2D, renderTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
			glGenRenderbuffers(1, &depthBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texWidth, texHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, App->renderer->depthBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
	}
	//drawing
	Draw();
	ImGui::GetWindowDrawList()->AddImage(
		(void*)renderTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + texWidth,
			ImGui::GetCursorScreenPos().y + texHeight),
		ImVec2(0, 1), ImVec2(1, 0));
}

void ModuleRender::drawGizmo() {
	ImGuizmo::Enable(true);
	ImGuizmo::SetDrawlist();
	ImVec2 pos = ImGui::GetWindowPos();
	ImGuizmo::SetRect(pos.x, pos.y, App->camera->screenWidth, App->camera->screenHeight);
	ImGuizmo::OPERATION transformType = ImGuizmo::TRANSLATE;
	switch (gizmoTransf) {
	case 0:
		transformType = ImGuizmo::TRANSLATE;
		break;
	case 1:
		transformType = ImGuizmo::ROTATE;
		break;
	case 2:
		transformType = ImGuizmo::SCALE;
		break;
	}
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

	ComponentTransform* objTransf = App->scene->objectSelected->transform;
	
	math::float4x4 modelMatrix = objTransf->transformMatrix;
	math::float4x4 viewMatrix = App->camera->frustum.ViewMatrix();
	math::float4x4 projectionMatrix = App->camera->projection;

	modelMatrix.Transpose();
	ImGuizmo::SetOrthographic(false);

	ImGuizmo::Manipulate((float*)&viewMatrix.Transposed(), (float*)&projectionMatrix.Transposed(), transformType, mCurrentGizmoMode, (float*)&modelMatrix, NULL, NULL, NULL, NULL);

	objTransf->setValues(modelMatrix.Transposed());
	//ImGui::SetCursorPos({ 20,30 });
}