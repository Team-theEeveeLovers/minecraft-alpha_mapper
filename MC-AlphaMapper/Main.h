#pragma once

#include <iostream>
// trickery to get around unresolved external errors caused by a "lack" of main function
// the macro by SDL to expand main to SDL_main breaks ninja
#define main main


bool initMain();

void exitMain();