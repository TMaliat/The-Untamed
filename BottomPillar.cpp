#include "BottomPillar.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

BottomPillar::~BottomPillar()
{
	SDL_DestroyTexture(image);
}

BottomPillar::BottomPillar()
{

}

BottomPillar::BottomPillar(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.y = PILLAR_Y_POS;
	currentPosition.x = SCREEN_WIDTH + rect.w;
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h - PILLAR_HEIGHT_OFFSET; //Pillar Height Offset only used to make it flush with bottom screen
}

bool BottomPillar::detectCollision(Character character)
{
	bool gameOver = false;
	//Take note that some tweakings have been made in regard each hti detection method to make the game feel better
	if (character.currentPosition.x + character.rect.w >= currentPosition.x - rect.w && character.currentPosition.x <= currentPosition.x + rect.w / 2 && character.currentPosition.y <= currentPosition.y && character.currentPosition.y >= currentPosition.y - BOTTOM_PILLAR_TOP_OFFSET)
	{
		gameOver = true;
	}

	return gameOver;
}

bool BottomPillar::operator== (const BottomPillar &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void BottomPillar::move(float deltaTime)
{
	currentPosition.x -= LEVEL_SPEED * deltaTime;
}