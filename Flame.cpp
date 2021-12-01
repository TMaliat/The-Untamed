#include "Flame.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;


#define SCREEN_WIDTH //the added part
#define Screen_height //the added part
Flame::~Flame()
{
	SDL_DestroyTexture(image);
}

Flame::Flame()
{

}

Flame::Flame(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.x = SCREEN_WIDTH + rect.w + randomX; //Where the object is placed on screen
	currentPosition.y = SCREEN_HEIGHT - BOTTOM_SPACE_OFFSET - randomY; //Where the object is placed screen
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h; //set dimensions of the object 
}

bool Flame::detectCollision(Character character)
{
	bool gameOver = false;

	if (character.currentPosition.x + character.rect.w / 2 >= currentPosition.x - FLAME_LEFT_OFFSET && character.currentPosition.x <= currentPosition.x + currentPosition.w / 2 && character.currentPosition.y - character.rect.h/2 <= currentPosition.y + rect.h/2 && character.currentPosition.y + character.rect.h/2 >= currentPosition.y - rect.h/2)
	{
		gameOver = true;
	}

	return gameOver;
}

bool Flame::operator== (const Flame &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void Flame::nextFrameAnimation()
{
	this->rect.x += flameFrameWidth;
}

bool Flame::reachedLastAnimation()
{
	bool reachedLastAnimation = false;

	if (rect.x >= flameTextureWidth-8)
		reachedLastAnimation = true;

	return reachedLastAnimation;
}

void Flame::move(float deltaTime)
{
	currentPosition.x -= (LEVEL_SPEED + 100) * deltaTime;
}