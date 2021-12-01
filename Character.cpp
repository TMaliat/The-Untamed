#include "Character.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

Character::Character()
{

}

Character::Character(SDL_Texture* i, int x, int y, int w, int h)
{
image = i;
yVel = 0;
inAir = false;
atMaxHeight = false;

currentPosition.x = 0;
currentPosition.y = 0;
currentPosition.w = 85;
currentPosition.h = 85;
rect.x = 0;
rect.y = 0;
rect.w = 8;
rect.h = 8;
}

bool Character::getinAir()
{
	return inAir;
}