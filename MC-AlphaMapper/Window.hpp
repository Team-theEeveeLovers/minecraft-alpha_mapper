#pragma once

#include "globals.h"
#include <SDL2/SDL.h>
/**
* @file Window.hpp
* @brief The header file for the window handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 1st, 2024
*/


class WINDOW {
public:
	// get the SDL_Window* context contained within
	SDL_Window* getWindow();

	/** 
	* @brief create a window context
	*
	* @param title - the title of the window
	* @param width - the width of the window
	* @param height - the height of the window
	* @param flags - the flags of the window, or leave blank for default "SHOWN" flag
	*/
	bool createWindow(const char* title, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN);

	bool createWindowSurface();

	SDL_Surface* getWindowSurface();

	/**
	 * @brief Draw and fill a rectangle using the window's software renderer
	 * @param rect - the rectangle to fill, or NULL to fill the whole window
	 * @param red - the red component of the fill color
	 * @param green - the green component of the fill color
	 * @param blue - the blue component of the fill color
	 * @param alpha - the alpha component of the fill color, where 0 is fully transparent, and 255 is fully opaque. Leave blank for fully opaque.
	 */
	void fillRect(const SDL_Rect* rect, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 0xFF);

	// update the window surface
	void updateWindowSurface();

	/**
	 * @brief Set the minimum size of the window, if resizable
	 * @param w - the minimum width
	 * @param h - the minimum height
	 */
	void setMinimumWindowSize(int w, int h);


	// destroy the window and free memory
	void destroyWindow();

protected:
	SDL_Window* SDLW = NULL;
	SDL_Surface* SDLW_S = NULL;
};