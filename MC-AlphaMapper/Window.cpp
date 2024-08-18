#include "Window.hpp"
/**
* @file Window.cpp
* @brief The source file for the window handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 1st, 2024
*/

SDL_Window* WINDOW::getWindow()
{
	return SDLW;
}

bool WINDOW::createWindow(const char* title, int width, int height, Uint32 flags)
{
	SDLW = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	if (SDLW == NULL) {
		SDL_LogError(0, "SDL window creation failed. SDL Error: %s\n", SDL_GetError());
		ASSERT(SDLW != NULL && "Creating SDL window failed.");
		return false;
	}
	else {
		SDL_SetWindowMinimumSize(SDLW, width, height);
		return true;
	}
}

bool WINDOW::createWindowSurface()
{
	if (SDLW == NULL) { ASSERT(SDLW != NULL && "Tried to create surface from an uninitalized window!"); return false; }
	else {
		SDLW_S = SDL_GetWindowSurface(SDLW);
		if (SDLW_S == NULL) {
			SDL_LogError(0, "Getting surface from SDL window failed. SDL Error: %s\n", SDL_GetError());
			ASSERT(SDLW_S != NULL && "Getting surface from SDL window failed.");
			return false;
		}
		else {
			return true;
		}
	}
}

SDL_Surface* WINDOW::getWindowSurface()
{
	if (SDLW == NULL) { ASSERT(SDLW != NULL && "Tried to get surface of an uninitalized window!"); }
	else if (SDLW_S == NULL) { createWindowSurface(); }
	return SDLW_S;
}

void WINDOW::fillRect(const SDL_Rect* rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	if (SDLW_S == NULL) { createWindowSurface(); }

	Uint32 color = 0x00;
	// if fully opaque, we don't need to map alpha
	if (alpha == 0xFF) {
		color = SDL_MapRGB(SDLW_S->format, red, green, blue);
	}
	else {
		color = SDL_MapRGBA(SDLW_S->format, red, green, blue, alpha);
	}
	SDL_FillRect(SDLW_S, rect, color);
}

void WINDOW::updateWindowSurface()
{
	if (SDLW == NULL) {
		ASSERT(SDLW != NULL && "Tried to update surface of an uninitalized window!");
	}
	else if (SDLW_S == NULL) {
		ASSERT(SDLW_S != NULL && "Tried to update uninitalized surface!");
	}
	else {
		if (SDL_UpdateWindowSurface(SDLW) < 0) {
			ASSERT(NULL && SDL_GetError() && "Failed to update window surface");
		}
	}
}


void WINDOW::setMinimumWindowSize(int w, int h)
{
	// make sure the user isn't attempting to set the minimum size of an uninitalized window
	if (SDLW == NULL) {
		ASSERT(SDLW != NULL && "Tried to change properties of an uninitalized window!");
	}
	else {
		SDL_SetWindowMinimumSize(SDLW, w, h);
	}
}

void WINDOW::destroyWindow()
{
	SDL_DestroyWindow(SDLW);
}