#include "TopPillar.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

TopPillar::~TopPillar()
{
	SDL_DestroyTexture(image);
}

TopPillar::TopPillar()
{

}

TopPillar::TopPillar(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.y = PILLAR_Y_POS - TOP_PILLAR_Y_OFFSET;
	currentPosition.x = SCREEN_WIDTH + rect.w;
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h - PILLAR_HEIGHT_OFFSET; //Pillar Height Offset only used to make it flush with bottom screen
}

bool TopPillar::detectCollision(Character character)
{
	bool gameOver = false;
	//if (character.currentPosition.x + character.rect.w + CHARACTER_WIDTH_OFFSET + PILLAR_HEIGHT_OFFSET >= currentPosition.x - rect.w / 2 && character.currentPosition.x - CHARACTER_WIDTH_OFFSET + PILLAR_WIDTH_OFFSET <= currentPosition.x + rect.w / 2 && character.currentPosition.y <= currentPosition.y + currentPosition.h)
		
	if (character.currentPosition.x + character.rect.w >= currentPosition.x - rect.w && character.currentPosition.x <= currentPosition.x + rect.w / 2 && character.currentPosition.y <= currentPosition.y + rect.h/2 + TOP_PILLAR_BOTTOM_OFFSET)
	{
		gameOver = true;
	}
	
	return gameOver;
}

bool TopPillar::operator== (const TopPillar &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void TopPillar::move(float deltaTime)
{
	currentPosition.x -= LEVEL_SPEED * deltaTime;
}