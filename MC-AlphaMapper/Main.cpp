#include "Main.h"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Gui.hpp"
#include "FileLoading.hpp"
#include "Types/Time.h"
#include "Types/VectorTypes.h"

const char* MC_MAPPER_VERSION = "Nightly 0.0.2";

int screen_width = 854 * 2, screen_height = 480 * 2;

WINDOW main_window;
RENDERER main_renderer;
ImGui_CONTEXT main_gui;

LEVEL_DATA currentLVLFile;
Time LevelLastPlay;

CHUNK_DATA zero_zero; // Spawn
CHUNK_DATA one_zero;
CHUNK_DATA two_zero;
CHUNK_DATA three_zero;
CHUNK_DATA four_zero;
CHUNK_DATA five_zero;
CHUNK_DATA six_zero;

CHUNK_DATA zero_oner;
CHUNK_DATA one_oner;
CHUNK_DATA two_oner;
CHUNK_DATA three_oner;
CHUNK_DATA four_oner;
CHUNK_DATA five_oner;
CHUNK_DATA six_oner;

// are we currently loading chunks?
bool loading_Chunks = false;
// the chunk currently being loaded
int loading_Chunk = 0;

vector2_int scroll;

//#define DEBUG_MULTITHREADING

bool initMain() {
	bool success = true;

	std::cout << "Minecraft Alpha Level Mapper by TtEL" << std::endl;
	std::cout << "Software Version: " << MC_MAPPER_VERSION << std::endl << std::endl;
	//Initialize SDL
	std::cout << "Initalizing SDL... ";
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0)
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
					else {
						ImGuiIO& io = ImGui::GetIO();
						io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
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

/**
 * @brief Shows this program's debug menu
 * @param open - A pointer to a bool which will be set to false when the window is closed. If set to NULL (or not set at all), the window's close button will not show.
 */
void showDebugMenu(bool* open = (bool*)0) {
	if (!ImGui::Begin("MC Alpha Mapper Debugger", open, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::End();
		return;
	}

	ImGui::Text("Scroll Y: %d", scroll.y);

	if (ImGui::Button("Useless Button"))
	{
		SDL_Log("Nothing happened!\n");
	}
	ImGui::End();
}


void renderBlockAsRect(BYTE blockID, int x = 0, int x_offset = 0, int y = 0) {
	SDL_Rect drawingRect = { 4+(16 * y), 4+(16 * x), 16, 16 };

	// is the block currently being drawn known?
	bool knownBlock = false;
	switch (blockID) {
	case AIR:
		main_renderer.setDrawColor(0xB1, 0xEB, 0xF1, 0x99);
		knownBlock = true;
		break;
	case LAVA_FLOWING: case LAVA_STILL:
		main_renderer.setDrawColor(0xFC, 0x77, 0x03);
		knownBlock = true;
		break;
	case WATER_FLOWING: case WATER_STILL:
		main_renderer.setDrawColor(0x03, 0x77, 0xFC);
		knownBlock = true;
		break;
	case STONE:
		main_renderer.setDrawColor(0x55, 0x55, 0x55);
		knownBlock = true;
		break;
	case GRASS:
		main_renderer.setDrawColor(0x00, 0xFF, 0x77);
		knownBlock = true;
		break;
	case DIRT:
		main_renderer.setDrawColor(0x5E, 0x3E, 0x07);
		knownBlock = true;
		break;
	case BEDROCK:
		main_renderer.setDrawColor(0x11, 0x11, 0x11);
		knownBlock = true;
		break;
	case SNOW_LAYER:
		main_renderer.setDrawColor(0xEE, 0xEE, 0xFF);
		knownBlock = true;
		break;
	case SAND:
		main_renderer.setDrawColor(0xDE, 0xD4, 0xA4);
		knownBlock = true;
		break;
	default:
		main_renderer.setDrawColor(0xFF, 0xAA, 0xFF);
		break;
	}
	int screenWidth = screen_width;
	if (screenWidth % 16 != 0) 
		screenWidth -= (screenWidth % 16);

	if (drawingRect.y > 2048) {
		int tempX = drawingRect.y;
		while (tempX > 2048) {
			drawingRect.x += 16;
			tempX -= 2048;
		}
		drawingRect.y = tempX;
	}

	drawingRect.x += x_offset;

	// blocks with "texture"
	switch (blockID) {
	case TORCH:
		knownBlock = true;

		drawingRect.h = 4;
		drawingRect.w = 6;

		drawingRect.x += 6;

		main_renderer.setDrawColor(0xFF, 0xE9, 0x57);
		main_renderer.fillRect(&drawingRect);

		drawingRect.h = 12;
		drawingRect.y += 4;
		main_renderer.setDrawColor(0x57, 0x3C, 0x08);
		main_renderer.fillRect(&drawingRect);

		break;
	case GRASS:
		knownBlock = true;

		// top layer
		drawingRect.h = 4;
		main_renderer.setDrawColor(0x00, 0xFF, 0x77);
		main_renderer.fillRect(&drawingRect);

		// bottom layer
		drawingRect.h = 12;
		drawingRect.y += 4;
		main_renderer.setDrawColor(0x5E, 0x3E, 0x07);
		main_renderer.fillRect(&drawingRect);
		break;
	case GRAVEL:
		knownBlock = true;

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0x5E, 0x5E, 0x5E, 0x5E);
		main_renderer.fillRect({ drawingRect.x + 2, drawingRect.y + 2, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 10, drawingRect.y + 10, 4, 4 });

		main_renderer.setDrawColor(0x5E, 0x5E, 0x5E, 0x85);
		main_renderer.fillRect({ drawingRect.x + 3, drawingRect.y + 9, 4, 4 });

		break;
	case BEDROCK:
		knownBlock = true;

		main_renderer.fillRect(&drawingRect);

		main_renderer.setDrawColor(0x5E, 0x5E, 0x5E, 0x5E);
		main_renderer.fillRect({ drawingRect.x + 2, drawingRect.y + 2, 4, 4 });

		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });
		break;
	case IRON_ORE:
		knownBlock = true;

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0xCC, 0x90, 0x7C, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });

		break;
	case COAL_ORE:
		knownBlock = true;

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0x22, 0x22, 0x22, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });
		
		break;

	case REDSTONE_ORE:
		knownBlock = true;

		main_renderer.setDrawColor(0x8C, 0x8C, 0x8C);
		main_renderer.fillRect(&drawingRect);

		// dots
		main_renderer.setDrawColor(0xFF, 0x22, 0x22, 0xCC);
		main_renderer.fillRect({ drawingRect.x + 4, drawingRect.y + 4, 4, 4 });
		main_renderer.fillRect({ drawingRect.x + 8, drawingRect.y + 8, 4, 4 });

		break;
	default:		
		if (blockID == SNOW_LAYER) { drawingRect.h = 8; drawingRect.y += 8; }
		main_renderer.fillRect(&drawingRect);
		break;
	}

	if (!knownBlock) {
		/// make a string with a hexadecimal representation of the block ID
		// initalize a buffer
		char HEX_[4] = { '\0', '\0', '\0', '\0'};
		// fill the buffer with the hex representation
		sprintf_s(HEX_, "%02X", blockID);
		// draw the buffer to the screen
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(drawingRect.x, drawingRect.y), IM_COL32(0, 0, 0, 255), HEX_);
	}
}

/**
 * @brief Function to close all chunks so I don't have to write it twice when I add more chunks to load
 */
void closeChunks() {
	zero_zero.closeFile();
	one_zero.closeFile();
	two_zero.closeFile();
	three_zero.closeFile();
	four_zero.closeFile();
	five_zero.closeFile();
	six_zero.closeFile();

	zero_oner.closeFile();
	one_oner.closeFile();
	two_oner.closeFile();
	three_oner.closeFile();
	four_oner.closeFile();
	five_oner.closeFile();
	six_oner.closeFile();
}

// we gotta define up here because std::threads can't be called with class member functions (but it can be called within)
auto loadChunks(std::string BASEpath) {
	std::string BASEpath_TEMP = BASEpath;
	std::string ChunkPath = BASEpath_TEMP;

	loading_Chunk = 1;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\1\\0\\c.1.0.dat");
	if (one_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 1,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\1\\1r\\c.1.-1.dat");
	if (one_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 1,-1 sucessfully.\n");
	}

	loading_Chunk = 2;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\2\\0\\c.2.0.dat");
	if (two_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 2,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\2\\1r\\c.2.-1.dat");
	if (two_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 2,-1 sucessfully.\n");
	}

	loading_Chunk = 3;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\3\\0\\c.3.0.dat");
	if (three_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 3,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\3\\1r\\c.3.-1.dat");
	if (three_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 3,-1 sucessfully.\n");
	}

	loading_Chunk = 4;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\4\\0\\c.4.0.dat");
	if (four_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 4,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\4\\1r\\c.4.-1.dat");
	if (four_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 4,-1 sucessfully.\n");
	}

	loading_Chunk = 5;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\5\\0\\c.5.0.dat");
	if (five_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 5,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\5\\1r\\c.5.-1.dat");
	if (five_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 5,-1 sucessfully.\n");
	}

	loading_Chunk = 6;
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::seconds(1));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\6\\0\\c.6.0.dat");
	if (six_zero.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 6,0 sucessfully.\n");
	}
#ifdef DEBUG_MULTITHREADING
	// Debug Delay
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
	ChunkPath = BASEpath_TEMP;
	ChunkPath.append("\\6\\1r\\c.6.-1.dat");
	if (six_oner.loadFile(ChunkPath)) {
		SDL_Log("Loaded chunk 6,-1 sucessfully.\n");
	}

	// we are no longer loading chunks
	loading_Chunks = false;
	// reset status value
	loading_Chunk = 0;
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
		// Is the MC Alpha Mapper Debugger window open?
		bool debgMCWindowOpen = false;

		// is the value in draw incrementing or decrementing?
		bool drawIncrementing = false;


		// file menu
		ImGui::FileBrowser fileDialog;
		std::string selectedfilepath;
		std::string currentFolder;

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
						if (ControlPressed && currentLVLFile.initalized) {
							currentLVLFile.closeFile();
							currentFolder.clear();
							closeChunks();
						}
						break;
					}
				}

				if (e.type == SDL_MOUSEMOTION && currentLVLFile.initalized) {
					// Is the left mouse button held?
					if (e.motion.state & SDL_BUTTON_LEFT) {
						ImGuiIO IM_IO = ImGui::GetIO();

						// if the mouse isn't inside UI
						if (!IM_IO.WantCaptureMouse) {
							scroll.y += e.motion.yrel;
							if (scroll.y > 30) {
								scroll.y = -97;
							}
							else if (scroll.y < -128) {
								scroll.y = -3;
							}
						}
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

							if (ImGui::MenuItem("Close File", "Ctrl+W", nullptr, currentLVLFile.initalized)) { 
								currentLVLFile.closeFile(); currentFolder.clear(); 
								closeChunks();
							}

							if (ImGui::MenuItem("Exit", "Alt+F4")) { quit = true; }


							ImGui::EndMenu();
						}
						if (ImGui::BeginMenu("Tools")) {
							if (ImGui::MenuItem("ImGui Demo Window", NULL, demoWindowOpen)) { demoWindowOpen = !demoWindowOpen; }
							if (ImGui::MenuItem("ImGui Metrics/Debugger", NULL, metrWindowOpen)) { metrWindowOpen = !metrWindowOpen; }
							if (ImGui::MenuItem("MC Alpha Mapper Debugger", NULL, debgMCWindowOpen)) { debgMCWindowOpen = !debgMCWindowOpen; }
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
					ImGui::Text("Current Status:"); ImGui::SameLine();

					if (currentLVLFile.initalized) {
						if (loading_Chunks) {
							ImGui::Text("Loading chunk %d/6", loading_Chunk);
							// + 1 because Spawn Chunk 
							// / 8.f because 7 total chunks and + 1 because the number keeps track of the CURRENTLY loading chunk (not the already loaded chunk)
							ImGui::ProgressBar((static_cast<float>(loading_Chunk + 1) / 8.f));
						}
						else {
							ImGui::Text("Loaded chunks"); 
							ImGui::NewLine();
						}


						
						ImGui::NewLine();
						ImGui::Text("Open File: %s", currentFolder.c_str());
						ImGui::NewLine();
						ImGui::Text("Last Play Time: %d/%d/%d", LevelLastPlay.toMonth(), LevelLastPlay.toDay(), LevelLastPlay.toYear());
						ImGui::SameLine(); ImGui::Text("at %d:%d:%d UTC", LevelLastPlay.toHour(), LevelLastPlay.toMinute(), LevelLastPlay.toSecond());
					}
					else {
						ImGui::Text("No file open");
					}
				}
				ImGui::End();

				if (currentLVLFile.initalized) {
					for (int i = 0; i < 2048; i++) {
						Byte curBlock = zero_zero.Blocks[i];
						Byte curBlock_behind = zero_oner.Blocks[i];
						int x_offset = (2047 - i) + scroll.y;

						if (zero_oner.init) {
							if (curBlock == AIR && curBlock_behind != AIR)
								renderBlockAsRect(curBlock_behind, x_offset);
						}
						renderBlockAsRect(curBlock, x_offset);


						if (one_zero.init) {
							curBlock = one_zero.Blocks[i];
							if (one_oner.init) {
								curBlock_behind = one_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 256);
							}
							renderBlockAsRect(curBlock, x_offset, 256);
						}
						if (two_zero.init) {
							curBlock = two_zero.Blocks[i];
							if (two_oner.init) {
								curBlock_behind = two_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 512);
							}

							renderBlockAsRect(curBlock, x_offset, 512);
						}
						if (three_zero.init) {
							curBlock = three_zero.Blocks[i];
							if (three_oner.init) {
								curBlock_behind = three_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 768);
							}

							renderBlockAsRect(curBlock, x_offset, 768);
						}
						if (four_zero.init) {
							curBlock = four_zero.Blocks[i];
							if (four_oner.init) {
								curBlock_behind = four_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 1024);
							}
							renderBlockAsRect(curBlock, x_offset, 1024);
						}
						if (five_zero.init) {
							curBlock = five_zero.Blocks[i];
							if (five_oner.init) {
								curBlock_behind = five_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 1280);
							}
							renderBlockAsRect(curBlock, x_offset, 1280);
						}
						if (six_zero.init) {
							curBlock = six_zero.Blocks[i];
							if (six_oner.init) {
								curBlock_behind = six_oner.Blocks[i];
								if (curBlock == AIR && curBlock_behind != AIR)
									renderBlockAsRect(curBlock_behind, x_offset, 1536);
							}
							renderBlockAsRect(curBlock, x_offset, 1536);
						}
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
				if (debgMCWindowOpen)
					showDebugMenu(&debgMCWindowOpen);

				fileDialog.Display();

				main_gui.renderPresent();
				main_renderer.renderPresent();

				if (fileDialog.HasSelected()) {
					main_renderer.setDrawColor(draw, draw, draw);
					main_renderer.renderClear();
					main_gui.newFrame();

					selectedfilepath = fileDialog.GetSelected().string();
					std::string containingDirectory = fileDialog.GetSelected().parent_path().string();
					currentFolder = fileDialog.GetSelected().parent_path().filename().string();
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
						// RESET SCROLL VALUE
						scroll = vector2_int();

						LevelLastPlay.unixTime = currentLVLFile.getLastPlayTime().value;

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
						main_gui.newFrame();

						const ImGuiViewport* viewport = ImGui::GetMainViewport();
						ImGui::SetNextWindowPos(viewport->WorkPos);
						ImGui::SetNextWindowSize(viewport->WorkSize);
						ImGui::Begin("Loading...", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
						ImGui::Text("Loading Chunks...");
						ImGui::Text("Please wait...");
						ImGui::End();


						main_gui.renderPresent();
						main_renderer.renderPresent();

						
						loading_Chunks = true;
						std::string ChunkPath = containingDirectory;
						ChunkPath.append("\\0\\0\\c.0.0.dat");
						if (zero_zero.loadFile(ChunkPath)) {
							SDL_Log("Loaded spawn chunk (0,0) sucessfully.\n");
						}
						ChunkPath = containingDirectory;
						ChunkPath.append("\\0\\1r\\c.0.-1.dat");
						if (zero_oner.loadFile(ChunkPath)) {
							SDL_Log("Loaded chunk 0,-1 sucessfully.\n");
						}

						std::thread chunkThread(loadChunks, containingDirectory);

						// detach thread because the chunk class's initalization flag will help dispel unwanted behavior
						// we don't need to wait (join) for it to complete
						chunkThread.detach();

						/***
						ChunkPath = containingDirectory;
						ChunkPath.append("\\1\\0\\c.1.0.dat");
						if (one_zero.loadFile(ChunkPath)) {
							SDL_Log("Loaded chunk 1,0 sucessfully.\n");
						}

						ChunkPath = containingDirectory;
						ChunkPath.append("\\2\\0\\c.2.0.dat");
						if (two_zero.loadFile(ChunkPath)) {
							SDL_Log("Loaded chunk 2,0 sucessfully.\n");
						}

						ChunkPath = containingDirectory;
						ChunkPath.append("\\3\\0\\c.3.0.dat");
						if (three_zero.loadFile(ChunkPath)) {
							SDL_Log("Loaded chunk 3,0 sucessfully.\n");
						}

						ChunkPath = containingDirectory;
						ChunkPath.append("\\4\\0\\c.4.0.dat");
						if (four_zero.loadFile(ChunkPath)) {
							SDL_Log("Loaded chunk 4,0 sucessfully.\n");
						}
						*/
					}
				}

			}
		}
		exitMain();
		return 0;
	}
}