#include "Gui.hpp"


/**
* @file Gui.cpp
* @brief The source file for the ImGui handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 6th, 2024
*/

bool ImGui_CONTEXT::createContext(void)
{
	GUI_CONTEXT = ImGui::CreateContext();
	if (!GUI_CONTEXT->Initialized) {
		return false;
		initalized = false;
	}
	else {
		return true;
		initalized = true;
	}
	backend_init = false;
}

bool ImGui_CONTEXT::initalizeForSDL2(WINDOW window, RENDERER renderer)
{
	backend_init = false;

	if (!initalized) 
		ASSERT(initalized && "Tried to initalize ImGui backend without initalizing ImGui first!");
	else if (!ImGui_ImplSDL2_InitForSDLRenderer(window, renderer)) 
		SDL_LogError(0, "Failed to initalize ImGui for SDL2.\n\n");
	else if (!ImGui_ImplSDLRenderer2_Init(renderer)) 
		SDL_LogError(0, "Failed to initalize ImGui for SDL_Renderer.\n\n");
	else {
		backend_init = true;
		initalizedRenderer = renderer;
	}

	return backend_init;
}

bool ImGui_CONTEXT::setCurrentContext()
{
	if (!initalized) {
		return false;
		ASSERT(initalized && "Tried to set the current ImGui context used by ImGui without initalizing ImGui!");
	}
	else {
		ImGui::SetCurrentContext(GUI_CONTEXT);
		return true;
	}
}

void ImGui_CONTEXT::newFrame(void)
{
	if (!backend_init)
		ASSERT(backend_init && "Tried to start a new ImGui frame without initalizing the render backend!");
	else if (!initalized)
		ASSERT(initalized && "Tried to start a new ImGui frame without initalizing ImGui!");
	else {
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}
}

void ImGui_CONTEXT::renderPresent(void)
{
	if (!backend_init)
		ASSERT(backend_init && "Tried to render an ImGui frame without initalizing the render backend!");
	else if (!initalized)
		ASSERT(initalized && "Tried to render an ImGui frame without initalizing ImGui!");
	else {
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), initalizedRenderer);
	}
}

void ImGui_CONTEXT::shutdownContext(void)
{
	if (backend_init)
		ImGui_ImplSDL2_Shutdown(); // shut down backend
	if (initalized)
		ImGui::DestroyContext(GUI_CONTEXT); // destroy context

	// free pointers
	GUI_CONTEXT = nullptr;
	initalizedRenderer = NULL;

	// reset bools
	initalized = false;
	backend_init = false;
	
}
