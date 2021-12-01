#include "Arrow.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

Arrow::~Arrow()
{

}

Arrow::Arrow()
{

}

Arrow::Arrow(SDL_Texture* i, int x, int y, int w, int h, Character character)
{
	shouldBeRendered = true;
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.y = 200;
	currentPosition.x = character.currentPosition.x;
	currentPosition.w = w - 215; //set dimensions of the object
	currentPosition.h = h - 215; //set dimensions of the object 
}

void Arrow::move(float deltaTime)
{

}