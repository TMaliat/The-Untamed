#pragma once
#include "Sprite.h"
#include "Character.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

//All gem spritesheets are 256 pixels wide, and 32 pixels high
//Each gem is 32 pixels wide, and 32 pixels high
// MAX HEIGHT that the player can reach the gem at is height ~290

class Gem : public Sprite
{
public:
	~Gem();
	Gem();
	Gem(SDL_Texture* image, int x, int y, int w, int h);
	bool operator==(const Gem & other);
	bool detectCollision(Character character);
	void nextFrameAnimation();
	void move(float deltaTime);
};