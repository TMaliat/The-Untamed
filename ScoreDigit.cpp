#include "ScoreDigit.h"

#include <iostream>
#include "GameVariables.h"
using namespace std;

ScoreDigit::ScoreDigit(SDL_Texture* i, int x, int y)
{
	image = i;
	moveRect.x = 0;
	moveRect.y = 0;
	moveRect.w = digitFrameWidth;
	moveRect.h = digitFrameHeight;

	spotRect.x = x;
	spotRect.y = y;
	spotRect.w = digitFrameWidth;
	spotRect.h = digitFrameHeight;
}

void ScoreDigit::nextFrameAnimation()
{
	moveRect.x += digitFrameWidth;
}

void ScoreDigit::setToStartAnimation()
{
	moveRect.x = 0;
}

bool ScoreDigit::reachedLastAnimation()
{
	bool lastAnimation = false;

	if (moveRect.x > digitTextureWidth-digitFrameWidth)
		lastAnimation = true;

	return lastAnimation;
}