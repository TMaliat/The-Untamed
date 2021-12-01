#include "SpriteGroup.h"
#include "Sprite.h"
#include "Character.h"
#include "GameVariables.h"
#include <iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

using namespace std;

vector <Sprite*> SpriteGroup::getSprites()
{
	return sprites;
}

void SpriteGroup::add(Sprite* newSprite)
{
	sprites.push_back(newSprite);
	groupSize = sprites.size();
}

/* void SpriteGroup::remove(Sprite sprite)
{
	for (int i = 0; i < groupSize; i++)
	{		if (*sprites[i] == sprite)
			sprites.erase(sprites.begin() + i);
	}
} */

void SpriteGroup::removeAll()
{
	for (int i = 0; i < groupSize; i++)
	{
		delete(sprites[i]);
	}
	sprites.clear();
	groupSize = 0;
}

void SpriteGroup::remove(Sprite* sprite)
{
	for (int i = 0; i < groupSize; i++)
	{
		if (*sprites[i] == sprite)
		{
			sprites.erase(sprites.begin() + i);
			groupSize--;
		}
		//NEED TO FIX THIS MEMORY MANAGEMENT AFTER!
	}
}

bool SpriteGroup::has(Sprite* sprite)
{
	for (int i = 0; i < groupSize; i++)
	{
		if (*sprites[i] == sprite)
		{
			return true;
		}
	}
}

void SpriteGroup::draw(SDL_Renderer* renderTarget)
{
	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize; i++)
		{
			if (sprites[i] != nullptr)
			{
				SDL_RenderCopy(renderTarget, sprites[i]->image, &sprites[i]->rect, &(sprites[i]->currentPosition));
				int blockPos = sprites[i]->currentPosition.x;
			}
		}
	}

}

void SpriteGroup::update(float deltaTime)
{
	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize; i++)
		{
			if (sprites[i] != nullptr)
			{
				sprites[i]->move(deltaTime);
			}
		}
	}
}

void SpriteGroup::nextFrameAnimation()
{
	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize; i++)
		{
			if (sprites[i] != nullptr)
			{
				sprites[i]->nextFrameAnimation();
			}
		}
	}
}

void SpriteGroup::setToStartAnimation()
{
	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize; i++)
		{
			if (sprites[i] != nullptr)
			{
				sprites[i]->rect.x = 0;
			}
		}
	}
}

void SpriteGroup::cleanup()
{
	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize; i++)
		{
			if (sprites[i] != nullptr)
			{
				if (sprites[i]->currentPosition.x <= -100)
				{
					remove(sprites[i]);
				}
			}
		}
	}
}

bool SpriteGroup::detectCollision(Character character)
{
	bool gameOver = false;

	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize && !gameOver; i++)
		{
			if (sprites[i] != nullptr)
			{
				gameOver = sprites[i]->detectCollision(character);
			}
		}
	}

	return gameOver;
}

bool SpriteGroup::reachedLastAnimation()
{
	bool reachedLastAnimation = false;

	if (!sprites.empty())
	{
		for (int i = 0; i < groupSize && !reachedLastAnimation; i++)
		{
			if (sprites[i] != nullptr)
			{
				reachedLastAnimation = sprites[i]->reachedLastAnimation();
			}
		}
	}

	return reachedLastAnimation;
}
