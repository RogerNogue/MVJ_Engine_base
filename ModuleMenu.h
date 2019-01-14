#ifndef __ModuleMenu_h__
#define __ModuleMenu_h__
#include "Module.h"
#include "imgui.h"

class ModuleMenu :
	public Module
{
public:
	ModuleMenu();
	~ModuleMenu();

	bool            Init();
	update_status   PreUpdate() override;
	update_status   Update() override;
	bool            CleanUp() override;
	void DrawMenu();
	void consoleLog(const char* str);
	struct ExampleAppLog
	{
		ImGuiTextBuffer     Buf;
		bool                ScrollToBottom;

		void    Clear() { Buf.clear(); }

		void    AddLog(const char* fmt, ...) //IM_PRINTFARGS(2)
		{
			va_list args;
			va_start(args, fmt);
			Buf.appendf(fmt, args);
			va_end(args);
			ScrollToBottom = true;
		}

		void    Draw(const char* title, bool* p_opened = NULL)
		{
			ImGui::Begin(title, p_opened);
			ImGui::TextUnformatted(Buf.begin());
			if (ScrollToBottom)
				ImGui::SetScrollHere(1.0f);
			ScrollToBottom = false;
			ImGui::End();
		}
	};

	//variables
	int frames;
	double lastFrameTime;
	double lastSecondTime;
	int logMSIterator;
	int logFPSIterator;
	float* fps_log = nullptr;
	float* ms_log = nullptr;
	ExampleAppLog console;
	float menubarWidth, menubarHeight;
private:
	void updateFramerates();
	//variables
	ImVec2 viewportSize;
	bool nameCopied = false;
};

#endif /* __ModuleMenu_h__ */
