#include "Sprite.h"
#include "Character.h"
#include <iostream>
using namespace std;

//bool Sprite::operator== (const Sprite &other)
//{
//	return (currentPosition.x == other.currentPosition.x && currentPosition.y == other.currentPosition.y);
//}

bool Sprite::detectCollision(Character character)
{
	//this function will never get called
	return true;
}

bool Sprite::operator== (Sprite* other)
{
	return (currentPosition.x == other->currentPosition.x && currentPosition.y == other->currentPosition.y);
}

void Sprite::nextFrameAnimation()
{

}

void Sprite::move(float deltaTime)
{

}

bool Sprite::reachedLastAnimation()
{
	return false;
}

void Sprite::setToStartAnimation()
{

}