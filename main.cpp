#include <iostream>
#include "Game.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

#undef main

int main()
{
	Game game;
	game.run();
	return 0;
}