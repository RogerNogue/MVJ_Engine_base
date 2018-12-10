#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "ModuleRender.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#include "ModuleCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleScene.h"
#include "Brofiler.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

ModuleMenu::ModuleMenu()
{
}


ModuleMenu::~ModuleMenu()
{
}

bool ModuleMenu::Init() {
	// Setup window
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(GLSL_VERSION);
	//setting up the vars for the logs
	logMSIterator = 0;
	logFPSIterator = 0;
	fps_log = new float[50];
	ms_log = new float[50];
	lastFrameTime = SDL_GetTicks();
	lastSecondTime = SDL_GetTicks();
	console.AddLog("Console log:\n");
	return true;
}
void ModuleMenu::updateFramerates() {
	double timeElapsed = SDL_GetTicks() - lastSecondTime;
	//fps calculation
	lastSecondTime = SDL_GetTicks();
	fps_log[logFPSIterator] = 1000.f/timeElapsed;
	++logFPSIterator;
	if (logFPSIterator > 49) logFPSIterator = 0;
	//ms calculation
	ms_log[logMSIterator] = (float)timeElapsed;
	lastFrameTime = SDL_GetTicks();
	//iterator increase
	++logMSIterator;
	if (logMSIterator > 49) logMSIterator = 0;
}

update_status ModuleMenu::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Update() {
	BROFILER_CATEGORY("Menu update", Profiler::Color::Teal)
	menubarHeight = App->camera->screenHeight/4.5;
	menubarWidth = App->camera->screenWidth/4.5;
	// Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::Text("Menu");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::Button("Documentation")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("Download latest")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("Report a bug")) {
				ShellExecute(NULL, "open", "https://github.com/RogerNogue/MVJ_Engine_base/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About")) {
			ImGui::BulletText("Engine name: GNOBLIN");
			ImGui::Text("Description");
			ImGui::BulletText("GNOBLIN allows you to create AAA-quality games with little to none effort.");
			ImGui::Text("Authors");
			ImGui::BulletText("Roger Nogue Ballbe.");
			ImGui::Text("Libraries");
			ImGui::BulletText("SDL.");
			ImGui::BulletText("IMGUI.");
			ImGui::BulletText("GLEW.");
			ImGui::Text("Licenses");
			if (ImGui::Button("SDL")) {
				ShellExecute(NULL, "open", "https://www.libsdl.org/license.php", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("IMGUI")) {
				ShellExecute(NULL, "open", "https://github.com/ocornut/imgui/blob/master/LICENSE.txt", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::Button("GLEW")) {
				ShellExecute(NULL, "open", "https://www.opengl.org/about/#11", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Exit")) {
			return UPDATE_STOP;
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::SetNextWindowPos(ImVec2(App->camera->screenWidth - menubarWidth, 20));
	ImGui::SetNextWindowSize(ImVec2(menubarWidth, App->camera->screenHeight - menubarHeight - 20));
	bool displaying = true;

	ImGui::Begin("Editor Properties", &displaying);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Application details"))
		{
			if (ImGui::CollapsingHeader("Performance information"))
			{
				ImGui::Text("Application Time = %d", SDL_GetTicks() / 1000);
				char* title = new char[50];
				updateFramerates();
				sprintf_s(title, 50, "Framerate %.1f", fps_log[logFPSIterator]);
				ImGui::PlotHistogram("", fps_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
				sprintf_s(title, 50, "Milliseconds %.1f", ms_log[logMSIterator]);
				ImGui::PlotHistogram("", ms_log, 50, 0, title, 0.0f, 100.0f, ImVec2(350, 100));
			}
			//global variables
			if (ImGui::CollapsingHeader("Globals"))
			{
				ImGui::Text("Screen Width = %.1f", App->camera->screenWidth);
				ImGui::Text("Screen Height = %.1f", App->camera->screenHeight);
				ImGui::Text("Fullscreen = %d", FULLSCREEN);
				ImGui::Text("VSYNC = %d", VSYNC);
				ImGui::Text("GLSL version = %s", GLSL_VERSION);
			}
			//module render exercise
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::BulletText("Update time = %.2lf", App->camera->updateTime);

				ImGui::Text("Cam declarations:");
				ImGui::BulletText("Camera = ( %.2f, %.2f, %.2f )", App->camera->frustum.pos.x, App->camera->frustum.pos.y, App->camera->frustum.pos.z);
				ImGui::BulletText("Up = ( %.2f, %.2f, %.2f )", App->camera->frustum.up.x, App->camera->frustum.up.y, App->camera->frustum.up.z);
				ImGui::BulletText("Front = ( %.2f, %.2f, %.2f )", App->camera->frustum.front.x, App->camera->frustum.front.y, App->camera->frustum.front.z);
				math::float3 side = math::Cross(App->camera->frustum.front, App->camera->frustum.up).Normalized();
				ImGui::BulletText("Side = ( %.2f, %.2f, %.2f )", side.x, side.y, side.z);
				ImGui::InputFloat("zNear Plane", &App->camera->frustum.nearPlaneDistance);
				ImGui::InputFloat("zFar Plane", &App->camera->frustum.farPlaneDistance);
				ImGui::InputFloat("keys movement speed", &App->camera->movementSpeed);
				ImGui::InputFloat("mouse rotation speed", &App->camera->mouseRotSpeed);
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Object inspector"))
		{
			if (App->scene->objectSelected == nullptr) {
				ImGui::Text("No object selected");
			}
			else {
				char name[150];
				strcpy_s(name, 150, App->scene->objectSelected->name);
				ImGui::PushID("GO name");
				ImGui::PushItemWidth(150);
				if (ImGui::InputText("", name, 150, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					App->scene->objectSelected->name = name;
				ImGui::PopItemWidth();
				ImGui::PopID();
				if (ImGui::Checkbox("Active", &App->scene->objectSelected->active)) {
					App->scene->objectSelected->activeToggled();
				}
				if (ImGui::Button("Toggle Bounding Box")) {
					App->scene->objectSelected->paintBB = !App->scene->objectSelected->paintBB;
				}
				if (ImGui::CollapsingHeader("Transform"))
				{
					float movementSpeed = 50;
					ImGui::Text("Position");
					ImGui::PushID("1");
					if (ImGui::SliderFloat("X", &App->scene->objectSelected->transform->positionValues.x, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("2");
					if (ImGui::SliderFloat("Y", &App->scene->objectSelected->transform->positionValues.y, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("3");
					if (ImGui::SliderFloat("Z", &App->scene->objectSelected->transform->positionValues.z, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();

					movementSpeed = 5;
					ImGui::Text("Rotation");
					ImGui::PushID("4");
					if (ImGui::SliderFloat("X", &App->scene->objectSelected->transform->rotationValues.x, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("5");
					if (ImGui::SliderFloat("Y", &App->scene->objectSelected->transform->rotationValues.y, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("6");
					if (ImGui::SliderFloat("Z", &App->scene->objectSelected->transform->rotationValues.z, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();

					movementSpeed = 2;
					ImGui::Text("Scale");
					ImGui::PushID("7");
					if (ImGui::SliderFloat("X", &App->scene->objectSelected->transform->scaleValues.x, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("8");
					if (ImGui::SliderFloat("Y", &App->scene->objectSelected->transform->scaleValues.y, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();
					ImGui::PushID("9");
					if (ImGui::SliderFloat("Z", &App->scene->objectSelected->transform->scaleValues.z, -movementSpeed, movementSpeed)) App->scene->objectSelected->transform->objectMoved = true;
					ImGui::PopID();

					if (ImGui::Button("Place at (0,0,0)")) {
						App->scene->objectSelected->transform->placeAt000();
					}

					App->scene->objectSelected->transform->Update();
				}
				if (App->scene->objectSelected->hasmesh) {
					if (ImGui::CollapsingHeader("Meshes")){
						for (int i = 0; i < App->scene->objectSelected->meshes.size(); ++i) {
							ImGui::Text("Mesh %d", i);
							if (ImGui::Checkbox("Active", &App->scene->objectSelected->meshes[i]->active)) {
								ImGui::Text("SHITO");
							}
							ImGui::BulletText("Triangle count = %.d", App->scene->objectSelected->meshes[i]->mesh.numVertices/3);
						}
					}
				}
				if (App->scene->objectSelected->hasmaterial) {
					if (ImGui::CollapsingHeader("Materials"))
					{
						for (int i = 0; i < App->scene->objectSelected->materials.size(); ++i) {
							ImGui::Text("Material %d", i);
							if (ImGui::Checkbox("Active", &App->scene->objectSelected->materials[i]->active)) {
								ImGui::Text("SHITO");
							}
							float matHeight = App->scene->objectSelected->materials[i]->material.sizeY;
							float matWidth = App->scene->objectSelected->materials[i]->material.sizeX;
							ImGui::Text("Material width = %f, height = %f", matWidth, matHeight);
							ImVec2 imageSize = { matHeight *(menubarWidth/ matWidth), menubarWidth };
							ImGui::Image((ImTextureID)App->scene->objectSelected->materials[i]->material.texture0, imageSize);
						}
					}
				}
				bool loadModule = true;
				ImGui::Button("Load model");

				if (loadModule)
				{
					if (ImGui::BeginPopupContextItem("create", 0))
					{
						if (ImGui::Button("House")) {
							App->modelLoader->unloadModels();
							App->modelLoader->loadModel(1, App->scene->objectSelected);
							loadModule = false;
						}
						if (ImGui::Button("Banana")) {
							App->modelLoader->unloadModels();
							App->modelLoader->loadModel(2, App->scene->objectSelected);
							loadModule = false;
						}
						if (ImGui::Button("Shield")) {
							App->modelLoader->unloadModels();
							App->modelLoader->loadModel(3, App->scene->objectSelected);
							loadModule = false;
						}
						ImGui::EndPopup();
					}
				}
				else loadModule = true;
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(menubarWidth, App->camera->screenHeight - menubarHeight - 20));
	ImGui::Begin("3D properties", &displaying);

	//going over all the menus
	//App info
	//Object explorer
	if (ImGui::CollapsingHeader("Object inspector"))
	{
		App->scene->setUpGameObjectMenu();

	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, App->camera->screenHeight - menubarHeight));
	ImGui::SetNextWindowSize(ImVec2(App->camera->screenWidth, menubarHeight));
	console.Draw("Console");

	//editor viewport
	ImGui::SetNextWindowPos(ImVec2(menubarWidth, 20));
	viewportSize = ImVec2(App->camera->screenWidth - 2 * menubarWidth, App->camera->screenHeight - menubarHeight - 20);
	ImGui::SetNextWindowSize(viewportSize);
	if (ImGui::Begin("Viewport"))
	{
		BROFILER_CATEGORY("Viewport drawing", Profiler::Color::Magenta)
		if (ImGui::BeginChild("Editor Camera", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
		{
			App->renderer->setUpViewport();
		}
		ImGui::EndChild();
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

update_status ModuleMenu::PostUpdate() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return UPDATE_CONTINUE;
}

bool ModuleMenu::CleanUp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	delete fps_log;
	delete ms_log;
	return true;
}

void ModuleMenu::consoleLog(const char* str) {
	char* b = new char[50];
	sprintf(b, str);
	App->menu->console.AddLog(b);
	delete b;
}

