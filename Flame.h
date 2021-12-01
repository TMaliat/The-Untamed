#pragma once
#include "Sprite.h"
#include "Character.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

class Flame : public Sprite
{
public:
	~Flame();
	Flame();
	Flame(SDL_Texture* image, int x, int y, int w, int h);
	bool operator==(const Flame  & other);
	bool detectCollision(Character character);
	void nextFrameAnimation();
	bool reachedLastAnimation();
	void move(float deltaTime);
};