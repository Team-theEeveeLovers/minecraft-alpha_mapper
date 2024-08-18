#include "Main.h"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Gui.hpp"
#include "FileLoading.hpp"
#include "Types/Time.h"

const char* MC_MAPPER_VERSION = "Alpha 0.0.1";

int screen_width = 854 * 2, screen_height = 480 * 2;

WINDOW main_window;
RENDERER main_renderer;
ImGui_CONTEXT main_gui;

LEVEL_DATA currentLVLFile;
Time LevelLastPlay;

CHUNK_DATA spawn;

bool initMain() {
	bool success = true;

	std::cout << "Minecraft Alpha Level Mapper by TtEL" << std::endl;
	std::cout << "Software Version: " << MC_MAPPER_VERSION << std::endl << std::endl;
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
					SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_BLEND);
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

	//// positioning
	style.WindowRounding = 4.0f;
	style.FrameRounding = 7.0f;
	style.GrabRounding = 7.0f;
	style.CircleTessellationMaxError = 0.375f;
	style.DisplayWindowPadding = ImVec2(32.f, 32.f);

	//// opacity
	///
	// disabled item opacity
	style.DisabledAlpha = 0.25f;

	//// colors
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
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.35f, 0.35f, 0.43f, 0.86f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.49f, 0.59f, 0.33f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.59f, 0.32f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.49f, 0.59f, 0.31f, 0.95f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.83f, 0.82f, 0.68f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.51f, 0.40f, 0.86f);
	colors[ImGuiCol_TabSelected] = ImVec4(0.31f, 0.51f, 0.71f, 0.97f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(1.00f, 0.54f, 0.54f, 0.42f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.08f, 0.94f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.41f, 0.22f, 0.00f, 1.00f);
}

void exitMain()
{
	std::cout << std::endl << std::endl << "Exiting..." << std::endl << std::endl;

	std::cout << "Destroying window..." << std::endl << std::endl;
	main_window.destroyWindow();
	std::cout << "Destroying renderer..." << std::endl << std::endl;
	main_renderer.destroyRenderer();

	currentLVLFile.closeFile();
	//Quit SDL subsystems
	SDL_Quit();

	main_gui.shutdownContext();

	std::cout << std::endl << std::endl << "Goodbye!" << std::endl << std::endl;
}

/**
 * @brief Shows this program's about menu
 * @param open - A pointer to a bool which will be set to false when the window is closed. If set to NULL (or not set at all), the window's close button will not show.
 */
void showAboutMenu(bool* open = (bool*)0) {
	if (!ImGui::Begin("About MC Alpha Mapper", open, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Minecraft Alpha Mapper");
	ImGui::SameLine(); ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "version %s", MC_MAPPER_VERSION);
	ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.3f, 1.0f), "by TtEL");
	ImGui::TextLinkOpenURL("GitHub", "https://github.com/Team-theEeveeLovers/minecraft-alpha_mapper");

	ImGui::Separator();
	ImGui::NewLine();



	ImGui::Indent();
	ImGui::SeparatorText("About");
	ImGui::Unindent();

	ImGui::TextWrapped("Minecraft Alpha Mapper is a very incomplete program for mapping Minecraft worlds saved using the Alpha Level Format.");
	ImGui::TextWrapped("This format was first used in Infdev 20100327 and was used up to Beta 1.2_02, after which the MC Region format became the standard. A notable exception occured with Infdev 20100624 where a new save format was used. However, it was immediately scrapped afterward due to immensely increased filesize.");



	ImGui::End();
}


void renderBlockAsRect(BYTE blockID, int x, int y = 0) {
	SDL_Rect drawingRect = { 4+(16 * x), 4+(16 * y), 16, 16 };
	switch (blockID) {
	case AIR:
		main_renderer.setDrawColor(0xB1, 0xEB, 0xF1, 0x99);
		break;
	case STONE:
		main_renderer.setDrawColor(0x55, 0x55, 0x55);
		break;
	case GRASS:
		main_renderer.setDrawColor(0x00, 0xFF, 0x77);
		break;
	case DIRT:
		main_renderer.setDrawColor(0x5E, 0x3E, 0x07);
		break;
	case BEDROCK:
		main_renderer.setDrawColor(0x11, 0x11, 0x11);
		break;
	case SNOW_LAYER:
		main_renderer.setDrawColor(0xEE, 0xEE, 0xFF);
		break;
	default:
		main_renderer.setDrawColor(0xFF, 0xAA, 0xFF);
		break;
	}
	int screenWidth = screen_width;
	if (screenWidth % 16 != 0) 
		screenWidth -= (screenWidth % 16);

	if (drawingRect.x > 2048) {
		int tempX = drawingRect.x;
		while (tempX > 2048) {
			drawingRect.y += 16;
			tempX -= 2048;
		}
		drawingRect.x = tempX;
	}

	// blocks with "texture"
	if (blockID == TORCH) {
		drawingRect.h = 4;
		drawingRect.w = 6;

		drawingRect.x += 6;

		main_renderer.setDrawColor(0xFF, 0xE9, 0x57);
		main_renderer.fillRect(&drawingRect);

		drawingRect.h = 12;
		drawingRect.y += 4;
		main_renderer.setDrawColor(0x57, 0x3C, 0x08);
		main_renderer.fillRect(&drawingRect);
	}
	else if (blockID == GRASS) {
		// top layer
		drawingRect.h = 4;
		main_renderer.setDrawColor(0x00, 0xFF, 0x77);
		main_renderer.fillRect(&drawingRect);

		// bottom layer
		drawingRect.h = 12;
		drawingRect.y += 4;
		main_renderer.setDrawColor(0x5E, 0x3E, 0x07);
		main_renderer.fillRect(&drawingRect);

	}
	else if (blockID == GRAVEL) {

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0x5E, 0x5E, 0x5E, 0x5E);
		main_renderer.fillRect({ drawingRect.x + 2, drawingRect.y + 2, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 10, drawingRect.y + 10, 4, 4 });

		main_renderer.setDrawColor(0x5E, 0x5E, 0x5E, 0x85);
		main_renderer.fillRect({ drawingRect.x + 3, drawingRect.y + 9, 4, 4 });

	}
	else if (blockID == IRON_ORE) {

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0xCC, 0x90, 0x7C, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });
	}
	else if (blockID == COAL_ORE) {

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0x22, 0x22, 0x22, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });
	}
	else if (blockID == REDSTONE_ORE) {

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0xFF, 0x22, 0x22, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });
	}
	else {
		if (blockID == SNOW_LAYER) { drawingRect.h = 8; drawingRect.y += 8; }
		main_renderer.fillRect(&drawingRect);
	}
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

		// Is the ImGui Demo window open?
		bool demoWindowOpen = false;
		// Is the ImGui Metrics/Debugger window open?
		bool metrWindowOpen = false;
		// Is the About ImGui window open?
		bool abutWindowOpen = false;
		// Is the About MC Alpha Mapper window open?
		bool abutMCWindowOpen = false;

		// is the value in draw incrementing or decrementing?
		bool drawIncrementing = false;


		// file menu
		ImGui::FileBrowser fileDialog;
		std::string selectedfilepath;

		fileDialog.SetTitle("Open level.dat");
		fileDialog.SetTypeFilters(
			{
				".dat"
			}
		);

		while (!quit) {
			// temporary event handler until we move event handling to a separate header
			while (SDL_PollEvent(&e)) { 
				main_gui.processEvents(&e);
				if (e.type == SDL_QUIT) quit = true; 

				if (e.type == SDL_KEYDOWN) {
					const Uint8* keyStates = SDL_GetKeyboardState(NULL);
					bool ControlPressed = (keyStates[SDL_SCANCODE_LCTRL] || keyStates[SDL_SCANCODE_RCTRL]);

					switch (e.key.keysym.sym) {
					case SDLK_o:
						if (ControlPressed)
							fileDialog.Open();
						break;
					case SDLK_w:
						if (ControlPressed && currentLVLFile.initalized)
							currentLVLFile.closeFile();
						break;
					}
				}
			}
			// if the above hasn't resulted in exiting
			if (!quit) {
				if (!currentLVLFile.initalized) {
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
				}
				else {
					draw = 0xFB;
				}
				if (!currentLVLFile.initalized) {
					main_renderer.setDrawColor(draw, draw, draw);
				}
				else {
					main_renderer.setDrawColor(0x00, 0xFB, 0xFF);
				}
				main_renderer.renderClear();
				main_gui.newFrame();

				ImGui::SetNextWindowPos(ImVec2(20.f, 20.f), ImGuiCond_Once);
				if (ImGui::Begin("Operations", NULL, ImGuiWindowFlags_MenuBar)) {
					if (ImGui::BeginMenuBar()) {
						if (ImGui::BeginMenu("File")) {


							if (ImGui::MenuItem("Open", "Ctrl+O")) { fileDialog.Open(); }

							if (ImGui::MenuItem("Save", "(Coming Soon!)", false, false)) {}

							if (ImGui::MenuItem("Save As...", "(Coming Soon!)", false, false)) {}

							if (ImGui::MenuItem("Close File", "Ctrl+W", nullptr, currentLVLFile.initalized)) { currentLVLFile.closeFile(); }

							if (ImGui::MenuItem("Exit", "Alt+F4")) { quit = true; }


							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Tools")) {
							if (ImGui::MenuItem("ImGui Demo Window", NULL, demoWindowOpen)) { demoWindowOpen = !demoWindowOpen; }
							if (ImGui::MenuItem("ImGui Metrics/Debugger", NULL, metrWindowOpen)) { metrWindowOpen = !metrWindowOpen; }
							if (ImGui::MenuItem("Settings", "(Coming Soon!)", false, false)) {}
							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Help")) {
							if (ImGui::MenuItem("User Guide", "(Coming Soon!)", false, false)) {}
							if (ImGui::MenuItem("About ImGui", NULL, abutWindowOpen)) { abutWindowOpen = !abutWindowOpen; }
							if (ImGui::MenuItem("About MC Alpha Mapper", "(W.I.P)", abutMCWindowOpen)) { abutMCWindowOpen = !abutMCWindowOpen; }
							ImGui::EndMenu();
						}
						ImGui::EndMenuBar();
					}
					ImGui::Text("Coming Soon!");
					if (ImGui::Button("Useless Button"))
					{
						SDL_Log("Nothing happened!\n");
					}

					if (currentLVLFile.initalized) {
						ImGui::NewLine();
						ImGui::NewLine();
						ImGui::Text("Open File:");
						ImGui::NewLine();
						ImGui::Text("Last Play Time: %d/%d/%d", LevelLastPlay.toMonth(), LevelLastPlay.toDay(), LevelLastPlay.toYear());
						ImGui::SameLine(); ImGui::Text("at %d:%d:%d UTC", LevelLastPlay.toHour(), LevelLastPlay.toMinute(), LevelLastPlay.toSecond());
					}
				}
				ImGui::End();

				if (currentLVLFile.initalized) {
					for (int i = 0; i < 1536; i++) {
						Byte curBlock = spawn.Blocks[i];
						renderBlockAsRect(curBlock, i);
					}
				}
				
				if (demoWindowOpen)
					ImGui::ShowDemoWindow(&demoWindowOpen);
				if (metrWindowOpen)
					ImGui::ShowMetricsWindow(&metrWindowOpen);
				if (abutWindowOpen)
					ImGui::ShowAboutWindow(&abutWindowOpen);
				if (abutMCWindowOpen)
					showAboutMenu(&abutMCWindowOpen);

				fileDialog.Display();

				main_gui.renderPresent();
				main_renderer.renderPresent();

				if (fileDialog.HasSelected()) {
					main_renderer.setDrawColor(draw, draw, draw);
					main_renderer.renderClear();
					main_gui.newFrame();

					selectedfilepath = fileDialog.GetSelected().string();
					std::string containingDirectory = fileDialog.GetSelected().parent_path().string();
					fileDialog.ClearSelected();

					const ImGuiViewport* viewport = ImGui::GetMainViewport();
					ImGui::SetNextWindowPos(viewport->WorkPos);
					ImGui::SetNextWindowSize(viewport->WorkSize);
					ImGui::Begin("Loading...", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
					ImGui::Text("Loading...");
					ImGui::Text("Please wait...");
					ImGui::End();

					main_gui.renderPresent();
					main_renderer.renderPresent();
					if (!currentLVLFile.loadFile(selectedfilepath)) {
						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "An error occured!", ("Couldn't open file " + selectedfilepath + "\nPlease check log for issue!").c_str(), main_window);
					}
					else {

						// POST LOAD: Draw new frame

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

						main_renderer.renderPresent();

						LevelLastPlay.unixTime = currentLVLFile.getLastPlayTime().value;

						std::string spawnChunkPath = containingDirectory.append("\\0\\0\\c.0.0.dat");
						spawn.loadFile(spawnChunkPath);
					}
				}

			}
		}
		exitMain();
		return 0;
	}
}