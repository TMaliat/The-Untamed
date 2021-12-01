//#include <SDL2/SDL.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

class ScoreDigit
{
public:

	ScoreDigit(SDL_Texture* image, int x, int y); //Takes the x and y coordinates of where the number should be rendered on screen
	SDL_Texture* image;
	SDL_Rect spotRect; //The location on screen that the moveRect renders to eg.) onesSpot, tensSpot, ...
	SDL_Rect moveRect; //The rect that advances through the digits texture grabbing the correct number
	void nextFrameAnimation();
	bool reachedLastAnimation();
	void setToStartAnimation();
	bool shouldBeRendered = false;

};
