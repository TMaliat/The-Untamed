#pragma once
//#include <SDL2/SDL.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include <iostream>
#include "Sprite.h"
#include "Game.h"
#include "Block.h"
#include "Arrow.h"
#include "FallingBlock.h"
#include "BottomPillar.h"
#include "TopPillar.h"
#include "Flame.h"
#include "Gem.h"
#include "Character.h"
#include "SpriteGroup.h"
#include "ScoreDigit.h"
#include "GameVariables.h"
#include <time.h>
#include <ctime>
#include<stdbool.h>

using namespace std;

enum class GameState {PLAY,EXIT, TITLE, EXIT_FROM_TITLE, SCORE_SCREEN, VICTORY};


class Game {

public:
	Game();
	~Game();
	//void titleScreen();
	void mainmenu();
	void scoreScreen();
	void renderScores(); //Render all the Score Digits that shouldBeRendered
	void resetScores(); //Turn all Score Digits shouldBeRendered field to false and set them back to 0.
	void setNewHighScore(); //Render a new highscore to the screen
	void renderHighScore();
	void incrementTimers();
	void setScoreDigitsToScorePosition();
	void setScoreDigitstoGamePosition();
	void flickerArrow();
	SDL_Texture* loadSingleTexture(string filePath, SDL_Renderer* renderTarget);
	void loadAllGameTextures();
	void run();
	void resetGameVariables();

	void levelOne();
	void levelTwo();
	void levelThree();
	void levelFour();
	void levelFive();
	void beatGame();
	void congratulationsScreen();

private:

	GameState gameState;

	void gameLoop();
	void processInput(); 
	void initSystems();
	int randomYPosition();
	int randomXPosition();
};
