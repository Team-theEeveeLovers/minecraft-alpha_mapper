#pragma once



#include "globals.h"
#include "Window.hpp"

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
	/// BASE FUNCTIONS
	// function to convert to SDL_Renderer*
	operator SDL_Renderer* () { return SDLR; };

	/**
	 * @brief Get the SDL_Renderer* context contained within the object
	 * @deprecated This function is deprecated, please use the auto type converter to get an SDL_Renderer* context instead.
	 * @return the SDL_Renderer* context contained within the current RENDERER object
	 */
	SDL_Renderer* getRenderer();

	/**
	 * @brief Creates a rendering context.
	 * @param window - the window to attach the rendering context to
	 * @param flags - flags to create the renderer with, defaults to creating a software renderer
	 * @param index - the index of the rendering device to create the context with, or leave blank for the first supported device
	 * @return true if renderer created successfully, or false if error occurred.
	 */
	bool createRenderer(WINDOW window, Uint32 flags = SDL_RENDERER_SOFTWARE, int index = -1);















	/// DRAWING FUNCTIONS
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

	/**
	 * @brief Renders a texture to the screen
	 * @param textre - The texture to render
	 * @param sourceRect - A clipping rect for cropping the texture, or NULL to use the whole texture
	 * @param destinationRect - A rect in screenspace to draw the texture to, or NULL for the whole screen. The texture will be stretched to the rectangle.
	 */
	void renderTexture(SDL_Texture* textre, SDL_Rect* sourceRect = NULL, SDL_Rect* destinationRect = NULL);

	/**
	 * @brief Renders a texture to the screen
	 * @param textre - The texture to render
	 * @param sourceRect - A clipping rect for cropping the texture, or {0,0,0,0} to use the whole texture
	 * @param destinationRect - A rect in screenspace to draw the texture to, or {0,0,0,0} for the whole screen. The texture will be stretched to the rectangle.
	 */
	void renderTexture(SDL_Texture* textre, SDL_Rect sourceRect = {0,0,0,0}, SDL_Rect destinationRect = { 0,0,0,0 });

	/**
	* @brief Renders a rectangle to the screen
	* @param rect - a pointer to the rectangle to render
	 */
	void renderRect(SDL_Rect* rect);
	/**
	 * @brief Renders a rectangle to the screen
	 * @param rect - the rectangle to render
	 */
	void renderRect(SDL_Rect rect);

	/**
	* @brief Renders and fills a rectangle to the screen
	* @param rect - a pointer to the rectangle to render
	*/
	void fillRect(SDL_Rect* rect);
	/**
	* @brief Renders and fills a rectangle to the screen
	* @param rect - the rectangle to render
	*/
	void fillRect(SDL_Rect rect);

	// renders everything that is currently buffered
	void renderPresent();





	/// UTIL

	/**
	 * @brief Converts an SDL_Surface* object to an SDL_Texture* in the renderer's native format
	 * @param srface - a pointer to an SDL_Surface structure
	 * @return The created texture, or NULL on error.
	 */
	SDL_Texture* convertSurfaceToTexture(SDL_Surface* srface);









	/// BASE FUNCTIONS
	// destroys the rendering context and frees memory
	void destroyRenderer();
protected:
	// The main rendering context
	SDL_Renderer* SDLR = NULL;

};