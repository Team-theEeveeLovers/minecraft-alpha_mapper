#pragma once



#include "globals.h"

/**
* @file Renderer.hpp
* @brief The header file for the render handler of the TtEL Minecraft Alpha Level Mapper
* @author Team theEeveeLovers Sean
* @date August 2nd, 2024
*/

class RENDERER {
private:
	// INTERNAL FUNCTION: check if the current render is valid, if not, returns false
	bool checkRenderer();
public:

	// function to convert to SDL_Renderer*
	operator SDL_Renderer* () { return SDLR; };

	// Get the SDL_Renderer* context contained within
	SDL_Renderer* getRenderer();

	/**
	 * @brief Creates a rendering context.
	 * @param window - the window to attach the rendering context to
	 * @param flags - flags to create the renderer with, defaults to creating a software renderer
	 * @param index - the index of the rendering device to create the context with, or leave blank for the first supported device
	 * @return true if renderer created successfully, or false if error occurred.
	 */
	bool createRenderer(SDL_Window* window, Uint32 flags = SDL_RENDERER_SOFTWARE, int index = -1);

	/**
	 * @brief Sets the drawing color of the renderer
	 * @param red - the red component of the draw color
	 * @param green - the green component of the draw color
	 * @param blue - the blue component of the draw color
	 * @param alpha - the alpha component of the draw color, where 0 is fully transparent, and 255 is fully opaque. Leave blank for fully opaque.
	 */
	void setDrawColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = SDL_ALPHA_OPAQUE);

	// clears the backbuffer of the renderer using the draw color, starting a new frame
	void renderClear();

	// renders everything that is currently buffered
	void renderPresent();


	// destroys the rendering context and frees memory
	void destroyRenderer();
protected:
	// The main rendering context
	SDL_Renderer* SDLR = NULL;

};