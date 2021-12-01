#pragma once
#include "Sprite.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

//The character sprite sheet is 161 pixels wide, and 31 pixels high
//Each animation is 32 pixels wide, and 31 pixels high

class Character: public Sprite
{
public:
	Character();
	Character (SDL_Texture* image, int x, int y, int w, int h);
	bool getinAir();
	bool inAir;
	bool atMaxHeight;
	int yVel;
};