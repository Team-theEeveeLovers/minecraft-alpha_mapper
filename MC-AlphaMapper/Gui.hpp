#pragma once
#include "globals.h"
#include "Window.hpp"
#include "Renderer.hpp"

// include imgui_internal in order to get access to variables in ImGuiContext* structures
#include <imgui_internal.h>

/**
* @file Gui.hpp
* @brief The header file for the ImGui handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 6th, 2024
*/


class ImGui_CONTEXT {
public:
	/**
	 * @brief Creates the ImGui context 
	 * @return True on success, or false on failure
	 */
	bool createContext(void);

	/**
	 * @brief Initalizes the context
	 * @param window - WINDOW type to initalize for
	 * @param renderer - RENDERER type to initalize for
	 * @return True on success, or false on failure
	 */
	bool initalizeForSDL2(WINDOW window, RENDERER renderer);

	/**
	 * @brief Sets this context as the current one used by ImGui.
	 * If you only have one context, you do not need to call this function.
	 * @return True on success, or false on failure
	 */
	bool setCurrentContext();

	/**
	 * @brief Process events on the ImGui side. Call this before processing events yourself.
	 * @param event - pointer to SDL_Event structure to process
	 */
	void processEvents(SDL_Event* event);

	/**
	 * @brief Starts a new frame on the current context used by ImGui. Run this *after* you run renderClear() on your renderer.
	 */
	void newFrame(void);


	/**
	 * @brief Renders everything that is currently buffered on the current context used by ImGui. Run this **before** you run renderPresent() on your renderer.
	 */
	void renderPresent(void);

	/**
	 * @brief Shuts down the context and frees memory
	 */
	void shutdownContext(void);

protected:
	// the main context structure
	ImGuiContext* GUI_CONTEXT = NULL;

	// is the context initalized?
	bool initalized = false;

	// is the SDL2 backend initalized?
	bool backend_init = false;

	// File signature using unused padding space in memory
	char signature[6] = { '\0', 'G', 'U', 'I', 'C', '\0'};

	// the SDL_Renderer* the context is initalized with, if any.
	SDL_Renderer* initalizedRenderer = NULL;
};