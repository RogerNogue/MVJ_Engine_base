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
	ImGui::Begin("Module Properties", &displaying);

	//going over all the menus
	//App info
	if (ImGui::CollapsingHeader("Application details"))
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
	//module window
	/*if (ImGui::CollapsingHeader("Window"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}
	//module menu
	if (ImGui::CollapsingHeader("Menu"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}
	//module render
	if (ImGui::CollapsingHeader("Render"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}
	//module textures
	if (ImGui::CollapsingHeader("Textures"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}
	//module input
	if (ImGui::CollapsingHeader("Input"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}*/
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
	//module program
	/*if (ImGui::CollapsingHeader("Program"))
	{
		ImGui::Text("Not much to be shown about this module.");
	}*/
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 20));
	ImGui::SetNextWindowSize(ImVec2(menubarWidth, App->camera->screenHeight - menubarHeight - 20));
	ImGui::Begin("3D properties", &displaying);

	//going over all the menus
	//App info
	if (ImGui::CollapsingHeader("Model loader"))
	{
		if (ImGui::Button("House")) {
			App->modelLoader->unloadModels();
			App->modelLoader->loadModel(1);
		}
		if (ImGui::Button("Banana")) {
			App->modelLoader->unloadModels();
			App->modelLoader->loadModel(2);
		}
		if (ImGui::Button("Shield")) {
			App->modelLoader->unloadModels();
			App->modelLoader->loadModel(3);
		}
		ImGui::Text("Current model properties:");
		ImGui::BulletText("Triangle count = %.d", App->modelLoader->currentModelTriangleCount);
		ImGui::BulletText("Position = ( %.2f, %.2f, %.2f )", App->camera->modelCenter.x, App->camera->modelCenter.y, App->camera->modelCenter.z);
	}
	//Object explorer
	if (ImGui::CollapsingHeader("Object inspector"))
	{
		//App->scene->baseObject

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
		if (ImGui::BeginChild("Editor Camera", ImVec2(0, 0), true, ImGuiWindowFlags_NoMove))
		{
			setUpViewport();
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

void ModuleMenu::setUpViewport() {
	App->renderer->Draw();
	if (App->camera->screenWidth != viewportSize.x || App->camera->screenHeight != viewportSize.y) {
		App->renderer->WindowResized(viewportSize.x, viewportSize.y);
	}
	ImGui::Image((ImTextureID)App->renderer->renderTexture, { (float)viewportSize.x, (float)viewportSize.y }, { 0,1 }, { 1,0 });

}