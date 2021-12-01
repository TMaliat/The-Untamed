#include "FallingBlock.h"
#include "Character.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

FallingBlock::~FallingBlock()
{
	SDL_DestroyTexture(image);
}

FallingBlock::FallingBlock()
{

}

FallingBlock::FallingBlock(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;
	yVel = 0;

    //w=45; //the extra part tanzeem
	//h=45; //the extra part tanzeem

	currentPosition.y = 0;
	currentPosition.x = x;
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h; //set dimensions of the object 
}

bool FallingBlock::detectCollision(Character character)
{
	bool gameOver = false;
	//Take note that some tweakings have been made in regard each hit detection method to make the game feel better
	if (character.currentPosition.x + character.rect.w >= currentPosition.x - rect.w / 2 && character.currentPosition.x <= currentPosition.x + rect.w / 2 && character.currentPosition.y <= currentPosition.y && character.currentPosition.y >= currentPosition.y - rect.h)
	{
		gameOver = true;
	}

	return gameOver;
}

bool FallingBlock::operator== (const FallingBlock &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void FallingBlock::move(float deltaTime)
{
	currentPosition.y += yVel * deltaTime;
	yVel += OBSTACLE_GRAVITY;
}