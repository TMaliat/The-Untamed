#pragma once
#include "Sprite.h"
#include "Character.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

//All pillars are 23 pixels wide, and 124 pixels high

class TopPillar : public Sprite
{
public:
	~TopPillar();
	TopPillar();
	TopPillar(SDL_Texture* image, int x, int y, int w, int h);
	bool operator==(const TopPillar & other);
	bool detectCollision(Character character);
	void move(float deltaTime);
};