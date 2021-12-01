#pragma once
//#include <SDL2/SDL.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

class Character;

class Sprite {
public:
	SDL_Texture* image;
	SDL_Rect rect; //Stores the properties of where the object is rendered, eg.) which (x,y) along with what w and h
	SDL_Rect currentPosition; //Keeps track of the sprites current position, eg.) its (x,y) coordainte and
	bool operator==(Sprite* other);
	virtual bool detectCollision(Character character);
	virtual void nextFrameAnimation();
	virtual bool reachedLastAnimation();
	virtual void move(float deltaTime);
	virtual void setToStartAnimation();
};
