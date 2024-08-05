#include "Renderer.hpp"

/**
* @file Renderer.cpp
* @brief The source file for the render handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 2nd, 2024
*/



bool RENDERER::checkRenderer()
{
	if (SDLR == NULL)
		return false;
	else
		return true;
}

SDL_Renderer* RENDERER::getRenderer()
{
	return SDLR;
}

bool RENDERER::createRenderer(SDL_Window* window, Uint32 flags, int index)
{
	SDLR = SDL_CreateRenderer(window, index, flags);
	if (window == NULL) {
		ASSERT(window != NULL && "Tried to create a renderer from an invalid window!");
		return false;
	}
	else if (SDLR == NULL) {
		SDL_LogError(0, "SDL renderer creation failed. SDL Error: %s\n", SDL_GetError());
		ASSERT(SDLR != NULL && "Creating SDL renderer failed.");
		return false;
	}
	else {
		return true;
	}
}

void RENDERER::setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	if (!checkRenderer()) {
		ASSERT(SDLR != NULL && "Tried to set draw color of an invalid renderer!");
	}
	else {
		SDL_SetRenderDrawColor(SDLR, red, green, blue, alpha);
	}
}

void RENDERER::renderClear()
{
	if (!checkRenderer()) {
		ASSERT(SDLR != NULL && "Tried to render using an invalid renderer!");
	}
	else {
		SDL_RenderClear(SDLR);
	}
}

void RENDERER::renderPresent()
{
	if (!checkRenderer()) {
		ASSERT(SDLR != NULL && "Tried to render using an invalid renderer!");
	}
	else {
		SDL_RenderPresent(SDLR);
	}
}

void RENDERER::destroyRenderer()
{
	SDL_DestroyRenderer(SDLR);
}
