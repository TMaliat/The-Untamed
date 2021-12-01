#include "Gem.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

Gem::~Gem()
{
	SDL_DestroyTexture(image);
}

Gem::Gem()
{

}

Gem::Gem(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.x = SCREEN_WIDTH + rect.w + randomX; //Where the object is placed on screen
	currentPosition.y = SCREEN_HEIGHT - BOTTOM_SPACE_OFFSET - rect.h - randomY; //Where the object is placed screen
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h; //set dimensions of the object 
}

bool Gem::detectCollision(Character character)
{
	bool collectedGem = false;
	//Take note that some tweakings have been made in regard each hti detection method to make the game feel better
	if (character.currentPosition.x + character.rect.w / 2 >= currentPosition.x - rect.w / 2 && character.currentPosition.x <= currentPosition.x + rect.w/2 && character.currentPosition.y - character.rect.h <= currentPosition.y && character.currentPosition.y + character.rect.h >= currentPosition.y)
	{
		image = nullptr;
		currentPosition.x = 0;
		currentPosition.y = 0;
		scoreTime += 100;
		collectedGem = true;
	}

	return collectedGem;
}

bool Gem::operator== (const Gem &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void Gem::nextFrameAnimation()
{
	this->rect.x += gemFrameWidth;
}

void Gem::move(float deltaTime)
{
	currentPosition.x -= LEVEL_SPEED * deltaTime;
}
