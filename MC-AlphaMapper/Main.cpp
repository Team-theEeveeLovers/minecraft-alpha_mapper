#include "Main.h"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Gui.hpp"


int screen_width = 640, screen_height = 480;

WINDOW main_window;
RENDERER main_renderer;
ImGui_CONTEXT main_gui;

bool initMain() {
	bool success = true;

	std::cout << "Minecraft Alpha Level Mapper by TtEL" << std::endl;
	std::cout << "Software Version: Alpha 0.0.1" << std::endl << std::endl;
	//Initialize SDL
	std::cout << "Initalizing SDL... ";
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "failure" << std::endl;
		SDL_LogError(0, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		std::cout << "success" << std::endl << std::endl;

		if (!main_gui.createContext()) {
			SDL_LogError(0, "ImGui could not initialize!\n\n");
			success = false;
		}
		else {
			std::cout << "Creating window..." << std::endl;
			if (!main_window.createWindow("MC Alpha Mapper", screen_width, screen_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)) {
				success = false;
			}
			else {
				std::cout << "Successfully created window." << std::endl << std::endl;
				std::cout << "Creating renderer..." << std::endl;
				if (!main_renderer.createRenderer(main_window, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
					success = false;
				}
				else {
					std::cout << "Successfully created renderer." << std::endl << std::endl;
					if (!main_gui.initalizeForSDL2(main_window, main_renderer)) {
						SDL_LogError(0, "ImGui SDL2 backend could not initialize!\n\n");
						success = false;
					}
				}
			}
		}
	}

	return success;
}

void initMainStyles() 
{
	// Get the style
	ImGuiStyle& style = ImGui::GetStyle();

	// positioning
	style.WindowRounding = 4.0f;
	style.FrameRounding = 7.0f;
	style.GrabRounding = 7.0f;
	style.CircleTessellationMaxError = 0.375f;

	// colors
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.03f, 0.11f, 0.03f, 0.80f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.27f, 0.04f, 0.82f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.55f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.08f, 0.00f, 0.51f);
	colors[ImGuiCol_Button] = ImVec4(0.53f, 0.25f, 0.00f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.48f, 0.28f, 0.07f, 0.91f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.53f, 0.11f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 1.00f, 0.31f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.32f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.38f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 0.90f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.49f, 0.60f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.58f, 0.59f, 0.65f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.43f, 0.75f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.59f, 0.33f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.59f, 0.32f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.59f, 0.31f, 0.95f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.83f, 0.82f, 0.68f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.51f, 0.40f, 0.86f);
	colors[ImGuiCol_TabSelected] = ImVec4(0.31f, 0.51f, 0.71f, 0.97f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(1.00f, 0.54f, 0.54f, 0.42f);
}

void exitMain()
{
	std::cout << std::endl << std::endl << "Exiting..." << std::endl << std::endl;

	std::cout << "Destroying window..." << std::endl << std::endl;
	main_window.destroyWindow();
	std::cout << "Destroying renderer..." << std::endl << std::endl;
	main_renderer.destroyRenderer();
	//Quit SDL subsystems
	SDL_Quit();

	main_gui.shutdownContext();

	std::cout << std::endl << std::endl << "Goodbye!" << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
	if (!initMain()) {
		std::cout << std::endl << "Initalization Failed." << std::endl;
		return -575;
	}
	else {
		std::cout << std::endl << "Initalization Success." << std::endl;

		initMainStyles();

		SDL_Event e; 
		bool quit = false;
		int draw = 0xFF;
		bool demoWindowOpen = false;

		// is the value in draw incrementing or decrementing?
		bool drawIncrementing = false;

		while (!quit) {
			// temporary event handler until we move event handling to a separate header
			while (SDL_PollEvent(&e)) { 
				main_gui.processEvents(&e);
				if (e.type == SDL_QUIT) quit = true; 
			}
			// if the above hasn't resulted in exiting
			if (!quit) {
					if (draw != 0x00 && !drawIncrementing) {
						draw--;
					}
					else if (draw != 0xFF && drawIncrementing) {
						draw++;
					}
					else {
						drawIncrementing = !drawIncrementing;
						if (drawIncrementing)
							draw++;
						else
							draw--;
					}
				
				main_renderer.setDrawColor(draw, draw, draw);
				main_renderer.renderClear();
				main_gui.newFrame();

				ImGui::SetNextWindowPos(ImVec2(20.f, 20.f), ImGuiCond_Once);
				if (ImGui::Begin("Operations")) {
					ImGui::Text("Coming Soon!");
					if (ImGui::Button("Show ImGui Demo Window"))
					{
						demoWindowOpen = !demoWindowOpen;
					}
				}
				ImGui::End();
				
				if (demoWindowOpen)
					ImGui::ShowDemoWindow(&demoWindowOpen);

				main_gui.renderPresent();
				main_renderer.renderPresent();
			}
		}
		exitMain();
		return 0;
	}
}