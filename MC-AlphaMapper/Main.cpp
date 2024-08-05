#include "Main.h"
#include "Window.hpp"
#include "Renderer.hpp"


int screen_width = 640, screen_height = 480;

WINDOW main_window;
RENDERER main_renderer;

bool initMain() {
	bool success = true;

	std::cout << "Minecraft Alpha Level Mapper by TtEL" << std::endl;
	std::cout << "Software Version: Alpha 0.0.1" << std::endl << std::endl;
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
		
		std::cout << "Creating window..." << std::endl;
		if (!main_window.createWindow("MC Alpha Mapper", screen_width, screen_height)) {
			success = false;
		}
		else {
			std::cout << "Successfully created window." << std::endl << std::endl;
			std::cout << "Creating renderer..." << std::endl;
			if (!main_renderer.createRenderer(main_window.getWindow(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
				success = false;
			}
			else {
				std::cout << "Successfully created renderer." << std::endl << std::endl;
			}
		}
	}

	return success;
}

void exitMain()
{
	std::cout << std::endl << std::endl << "Exiting..." << std::endl << std::endl;

	std::cout << "Destroying window..." << std::endl << std::endl;
	main_window.destroyWindow();
	std::cout << "Destroying renderer..." << std::endl << std::endl;
	main_renderer.destroyRenderer();
	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	if (!initMain()) {
		std::cout << std::endl << "Initalization Failed." << std::endl;
		return -575;
	}
	else {
		std::cout << std::endl << "Initalization Success." << std::endl;

		
		SDL_Event e; 
		bool quit = false;
		int draw = 0xFF;

		// is the value in draw incrementing or decrementing?
		bool drawIncrementing = false;

		while (!quit) {
			// temporary event handler until we move event handling to a separate header
			while (SDL_PollEvent(&e)) { 
				if (e.type == SDL_QUIT) quit = true; 
			}
			// if the above hasn't resulted in exiting
			if (!quit) {
					if (draw != 0x00 && !drawIncrementing) {
						draw--;
					}
					else if (draw != 0xFF && drawIncrementing) {
						draw++;
					}
					else {
						drawIncrementing = !drawIncrementing;
						draw++;
					}
				
				main_renderer.setDrawColor(draw, draw, draw);
				main_renderer.renderClear();








				main_renderer.renderPresent();
			}
		}
		exitMain();
	}
}