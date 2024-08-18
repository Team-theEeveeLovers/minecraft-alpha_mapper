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

bool RENDERER::createRenderer(WINDOW window, Uint32 flags, int index)
{
	SDL_Window* mWindow = window;
	SDLR = SDL_CreateRenderer(mWindow, index, flags);
	if (mWindow == NULL) {
		ASSERT(mWindow != NULL && "Tried to create a renderer from an invalid window!");
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

void RENDERER::renderTexture(SDL_Texture* textre, SDL_Rect* sourceRect, SDL_Rect* destinationRect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Uninitalized renderer provided!");
	}
	else {
		SDL_RenderCopy(SDLR, textre, sourceRect, destinationRect);
	}
}

void RENDERER::renderTexture(SDL_Texture* textre, SDL_Rect sourceRect, SDL_Rect destinationRect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Uninitalized renderer provided!");
	}
	else {
		SDL_Rect* sourceRect_PTR = NULL;
		if (!(sourceRect.x == 0 && sourceRect.y == 0 && sourceRect.w == 0 && sourceRect.h == 0)) {
			sourceRect_PTR = &sourceRect;
		}
		SDL_Rect* destinationRect_PTR = NULL;
		if (!(destinationRect.x == 0 && destinationRect.y == 0 && destinationRect.w == 0 && destinationRect.h == 0)) {
			destinationRect_PTR = &destinationRect;
		}
		SDL_RenderCopy(SDLR, textre, sourceRect_PTR, destinationRect_PTR);
	}
}

void RENDERER::renderRect(SDL_Rect* rect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Tried to render a rectangle using an invalid renderer!");
	}
	else {
		SDL_RenderDrawRect(SDLR, rect);
	}
}

void RENDERER::renderRect(SDL_Rect rect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Tried to render a rectangle using an invalid renderer!");
	}
	else {
		SDL_RenderDrawRect(SDLR, &rect);
	}
}

void RENDERER::fillRect(SDL_Rect* rect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Tried to render a rectangle using an invalid renderer!");
	}
	else {
		SDL_RenderFillRect(SDLR, rect);
	}
}

void RENDERER::fillRect(SDL_Rect rect)
{
	if (!checkRenderer()) {
		throw std::invalid_argument("Tried to render a rectangle using an invalid renderer!");
	}
	else {
		SDL_RenderFillRect(SDLR, &rect);
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

SDL_Texture* RENDERER::convertSurfaceToTexture(SDL_Surface* srface)
{

	SDL_Texture* outTex = NULL;

	if (!checkRenderer()) {
		throw std::invalid_argument("Uninitalized renderer provided!");
		return outTex;
	}

	if (srface == nullptr) {
		throw std::invalid_argument("Invalid surface provided!");
		return outTex;
	}

	outTex = SDL_CreateTextureFromSurface(SDLR, srface);

	if (outTex == nullptr) {
		SDL_LogError(0, "Failed to create a texture from a surface! SDL_Error: %s\n", SDL_GetError());
		return outTex; // TODO: REDUNDANT???
	}


	return outTex;
}

void RENDERER::destroyRenderer()
{
	SDL_DestroyRenderer(SDLR);
}
