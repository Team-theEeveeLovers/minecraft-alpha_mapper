#include "Main.h"
#include "Window.hpp"

int screen_width = 640, screen_height = 480;

WINDOW main_window;

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
			if (!main_window.createWindowSurface()) {
				success = false;
			}
			else {
				std::cout << "Successfully created window." << std::endl << std::endl;
			}
		}
	}

	return success;
}

void exitMain()
{
	main_window.destroyWindow();
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

		// this value alternates between true/false every frame
		bool alternating = false;

		// is the value in draw incrementing or decrementing?
		bool drawIncrementing = false;

		while (!quit) {
			// temporary event handler until we move event handling to a separate header
			while (SDL_PollEvent(&e)) { 
				if (e.type == SDL_QUIT) quit = true; 
			}
			// if the above hasn't resulted in exiting
			if (!quit) {
				alternating = !alternating;
				if (alternating) {
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
				}

				main_window.fillRect(NULL, draw, draw, draw);

				main_window.updateWindowSurface();
			}
		}
		exitMain();
	}
}