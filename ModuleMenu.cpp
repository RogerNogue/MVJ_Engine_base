#include "Globals.h"
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
			
		}
		//module menu
		if (ImGui::CollapsingHeader("Menu"))
		{

		}
		//module render
		if (ImGui::CollapsingHeader("Render"))
		{

		}
		//module textures
		if (ImGui::CollapsingHeader("Textures"))
		{

		}
		//module input
		if (ImGui::CollapsingHeader("Input"))
		{

		}
		//module render exercise
		if (ImGui::CollapsingHeader("Render Exercise"))
		{

		}
		//module program
		if (ImGui::CollapsingHeader("Program"))
		{

		}
		/*
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		*/
		
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

