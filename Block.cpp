#include "Block.h"
#include <iostream>
#include "GameVariables.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

Block::~Block()
{
	SDL_DestroyTexture(image);
}

Block::Block()
{

}

Block::Block(SDL_Texture* i, int x, int y, int w, int h)
{
	image = i;
	rect.x = 0; //Start at the top-left corner of the spritesheet
	rect.y = 0; //Start at the top-left corner of the spritesheet
	rect.w = w;
	rect.h = h;

	currentPosition.y = BLOCK_Y_POS;
	currentPosition.x = SCREEN_WIDTH + rect.w;
	currentPosition.w = w; //set dimensions of the object
	currentPosition.h = h; //set dimensions of the object 
}

bool Block::detectCollision(Character character)
{
	bool gameOver = false;
	//Take note that some tweakings have been made in regard each hti detection method to make the game feel better
	if (character.currentPosition.x + character.rect.w >= currentPosition.x - rect.w / 2 && character.currentPosition.x <= currentPosition.x + rect.w / 2 && character.currentPosition.y <= currentPosition.y && character.currentPosition.y >= currentPosition.y - rect.h)
	{
		gameOver = true;
	}

	return gameOver;
}

bool Block::operator== (const Block &other)
{
	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
}

void Block::move(float deltaTime)
{
	currentPosition.x -= LEVEL_SPEED * deltaTime;
}