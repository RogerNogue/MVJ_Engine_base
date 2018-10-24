#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleMenu.h"
#include "ModuleRender.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include <stdio.h>
#include <SDL.h>
#include "ModuleRenderExercise.h"

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
	return true;
}

update_status ModuleMenu::PreUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleMenu::Update() {

	bool show_demo_window = false;
	if (show_demo_window)ImGui::ShowDemoWindow(&show_demo_window);
	
	else{
		// Menu
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("About"))
		{
			ImGui::BulletText("Engine name: DESPACITO 2");
			ImGui::Text("Description");
			ImGui::BulletText("DESPACITO 2 allows you to create AAA-quality games with little to none effort.");
			ImGui::Text("Authors");
			ImGui::BulletText("Roger Nogué Ballbé.");
			ImGui::Text("Libraries");
			ImGui::BulletText("SDL.");
			ImGui::BulletText("IMGUI.");
			ImGui::BulletText("GLEW.");
			ImGui::Text("Licenses");
			ImGui::BulletText("SDL: https://www.libsdl.org/license.php");
			ImGui::BulletText("IMGUI: https://github.com/ocornut/imgui/blob/master/LICENSE.txt");
			ImGui::BulletText("GLEW: https://www.opengl.org/about/#11");
			ImGui::Separator();
		}
		
		//going over all the menus
		//global variables
		if (ImGui::CollapsingHeader("Globals"))
		{
			ImGui::Text("Screen Width = %d", SCREEN_WIDTH);
			ImGui::Text("Screen Height = %d", SCREEN_HEIGHT);
			ImGui::Text("Fullscreen = %d", FULLSCREEN);
			ImGui::Text("VSYNC = %d", VSYNC);
			ImGui::Text("GLSL version = %s", GLSL_VERSION);
		}
		//module window
		if (ImGui::CollapsingHeader("Window"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		//module menu
		if (ImGui::CollapsingHeader("Menu"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		//module render
		if (ImGui::CollapsingHeader("Render"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		//module textures
		if (ImGui::CollapsingHeader("Textures"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		//module input
		if (ImGui::CollapsingHeader("Input"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		//module render exercise
		if (ImGui::CollapsingHeader("Render Exercise"))
		{
			ImGui::Text("Cam declarations:");
			ImGui::BulletText("Camera position = ( %f, %f, %f )", App->exercise->cam.x, App->exercise->cam.y, App->exercise->cam.z);
			ImGui::BulletText("vrp position = ( %f, %f, %f )", App->exercise->vrp.x, App->exercise->vrp.y, App->exercise->vrp.z);
			ImGui::BulletText("up directions = ( %f, %f, %f )", App->exercise->up.x, App->exercise->up.y, App->exercise->up.z);
		}
		//module program
		if (ImGui::CollapsingHeader("Program"))
		{
			ImGui::Text("Not much to be shown about this module yet.");
		}
		
	}
	

	//ImGui::End();
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
	return true;
}

