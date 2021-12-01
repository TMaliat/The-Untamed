#include "Game.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<stdbool.h>


/*****************
 * SPEED CONSTANTS
 *****************/
float const MOVE_LEFT = -300;
float const MOVE_RIGHT = 370;
float const MOVE_UP = -1000;
float const MOVE_DOWN = 500;
float const END_GAME_MOVE_RIGHT = 500;
float const GRAVITY = 100;
float const MAX_HEIGHT = 400;
float const BOTTOM_SPACE_OFFSET = 50;

/*****************
* OFFSET CONSTANTS
*****************/
int const SCREEN_WIDTH = 1000;
int const SCREEN_HEIGHT = 625;
int const BOTTOM_PILLAR_TOP_OFFSET = 50;
int const FLAME_LEFT_OFFSET = 25;
int const BLOCK_Y_POS = SCREEN_HEIGHT - 45 - BOTTOM_SPACE_OFFSET;
int const PILLAR_Y_POS = SCREEN_HEIGHT - 85 - BOTTOM_SPACE_OFFSET;
int const TOP_PILLAR_BOTTOM_OFFSET = 10;
int const PILLAR_HEIGHT_OFFSET = 50;
int const PILLAR_WIDTH_OFFSET = 10;
int const FALLING_BLOCK_START_X = 250;
int const DIGIT_Y_POSITION = 20;
int const HIGH_SCORE_X_STARTING_POSITION = 185;
int const SCORE_SCREEN_DIGIT_Y_POSITION = 300;

/*********************
* SPAWN RATE CONSTANTS
**********************/
int const LEVEL_1_SPAWN_RATE = 140;
int const LEVEL_1_INTERVAL = LEVEL_1_SPAWN_RATE*8;
int const LEVEL_2_SPAWN_RATE = 130;
int const LEVEL_2_INTERVAL = LEVEL_2_SPAWN_RATE*9;
int const LEVEL_3_SPAWN_RATE = 100;
int const LEVEL_3_INTERVAL = LEVEL_3_SPAWN_RATE*11;
int const LEVEL_4_SPAWN_RATE = 90;
int const LEVEL_4_INTERVAL = LEVEL_4_SPAWN_RATE*13;
int const LEVEL_5_SPAWN_RATE = 75;
int const LEVEL_5_INTERVAL = LEVEL_5_SPAWN_RATE*15;
int const BACKGROUND_APPEAR_TIMER = 30000;

/*********************
* MENU PART
**********************/
SDL_Texture* menu_tex;
SDL_Texture* credit_tex;
SDL_Texture* help_tex;
SDL_Texture* quit_tex;
SDL_Surface* menu_surface;
TTF_Font* font;

/**************
* GAME TEXTURES
***************/
SDL_Texture* runningMan;
SDL_Texture* brownBlock;
SDL_Texture* woodenBlock;
SDL_Texture* yellowBlock;
SDL_Texture* steelBlock;
SDL_Texture* diamondBlock;
SDL_Texture* brownBlockBroken;
SDL_Texture* woodenBlockBroken;
SDL_Texture* yellowBlockBroken;
SDL_Texture* steelBlockBroken;
SDL_Texture* diamondBlockBroken;
SDL_Texture* title;
SDL_Texture* background;
SDL_Texture* backgroundFlipped;
SDL_Texture* backgroundScore;
SDL_Texture* backgroundBeatGame;
SDL_Texture* highScoreText;
SDL_Texture* collectedGemText;
SDL_Texture* digits;
SDL_Texture* brownPillar;
SDL_Texture* woodenPillar;
SDL_Texture* yellowPillar;
SDL_Texture* steelPillar;
SDL_Texture* diamondPillar;
SDL_Texture* blueGem;
SDL_Texture* greenGem;
SDL_Texture* yellowGem;
SDL_Texture* redGem;
SDL_Texture* blackGem;
SDL_Texture* redFlame;
SDL_Texture* blueFlame;
SDL_Texture* purpleFlame;
SDL_Texture* redArrow;

/*************
* SCORE DIGITS
**************/
ScoreDigit* ones;
ScoreDigit* tens;
ScoreDigit* hundreds;
ScoreDigit* thousands;
ScoreDigit* tenThousands;
ScoreDigit* hundredThousands;
ScoreDigit* millions;
ScoreDigit* highScoreOnes;
ScoreDigit* highScoreTens;
ScoreDigit* highScoreHundreds;
ScoreDigit* highScoreThousands;
ScoreDigit* highScoreTenThousands;
ScoreDigit* highScoreHundredThousands;
ScoreDigit* highScoreMillions;

/***********
* GAME RECTS
************/
SDL_Rect backgroundRect;
SDL_Rect backgroundRectFlipped;
SDL_Rect collectedGemRect;
SDL_Rect highScoreRect;
SDL_Rect cameraRect;

/**************
* SPRITE GROUPS
***************/
SpriteGroup obstacles; //All the blocks/pillars
SpriteGroup gems; //All the gems
SpriteGroup flames; //All the flames

/***************************
* TEXTURE / FRAME DIMENTIONS
****************************/
int characterFrameWidth; //spritesheet's width in # of animations
int characterFrameHeight; //spritesheet's height in # of animations
int characterTextureWidth;
int characterTextureHeight;
int gemFrameWidth;
int gemFrameHeight;
int gemTextureWidth;
int gemTextureHeight;
int arrowFrameWidth;
int arrowFrameHeight;
int arrowTextureWidth;
int arrowTextureHeight;
int blockTextureWidth;
int blockTextureHeight;
int blockFrameWidth;
int blockFrameHeight;
int pillarTextureWidth;
int pillarTextureHeight;
int pillarFrameWidth;
int pillarFrameHeight;
int flameTextureWidth;
int flameTextureHeight;
int flameFrameWidth;
int flameFrameHeight;
int digitTextureWidth;
int digitTextureHeight;
int digitFrameWidth;
int digitFrameHeight;
int highScoreTextureWidth;
int highScoreTextureHeight;
int collectedGemTextureWidth;
int collectedGemTextureHeight;

/*********************
* SDL GLOBAL VARIABLES
**********************/
SDL_Window* window;
SDL_Surface* screen;
SDL_Renderer* renderTarget;

/*************
* TIME GLOBALS
**************/
int currTime = 0;
int prevTime = 0;
int scoreTime = 0;
int collectedGemTextTimer = 0;
int currentScoreTimer = 0; //Merely used to know when a high score is achieved.
int highScoreTimer = 0; // Uses a counting number, doesn't match up with rendering score.
int backgroundTimer = 0; //Used to time the switch of the background between levels
float deltaTime = 0; //Change in time
float frameTime = 0;
float flameFrameTime = 0;
float scoreFrameTime = 0;
float characterAnimationTime = .16; //The amount of time before the character resets its animation
float flameAnimationTime = .08; //The amount of time before a flame resets its animation
float scoreAnimationTime = .05; //The amount of time before the score clock moves to the next digit
int arrowFlickerTimer = 0;
int blockDelayTimer = 0; //Constantly counts up until an obstacle spawn occurs, then gets reset to 0.
int gemDelayTimer = 0; //Constantly counts up until a gem spawn occurs, then gets reset to 0.
int fallingBlockDelayTimer = 0; //Constantly counts up until a gem spawn occurs, then gets reset to 0.
int flameDelayTimer = 0; //Constantly counts up until a gem spawn occurs, then gets reset to 0.
int gemSpawnRate = LEVEL_1_SPAWN_RATE * 2; //BEGINS AS THIS VALUE but changes
int flameSpawnRate = LEVEL_1_SPAWN_RATE * 2; //BEGINS AS THIS VALUE but changes
int fallingBlockSpawnRate = LEVEL_1_SPAWN_RATE * 2; //BEGINS AS THIS VALUE but changes
int endGameDelayClock = 100; //N loop rotations before the character begins advancing

/*****************
* GLOBAL VARIABLES
******************/
bool backgroundLevelSet = false;
bool ghostTitleScreenBlock = true; //Used to prevent stupid first block from spawning
bool canSpawnBlock;
int randoms = 0;
int randomX = 0; //Used for spawning gems / flames at a good distance between blocks and pillars
int randomY = 0; //Used for spawning gems / flames at a height that the character can collect / be hit by
bool fallingBlockSpawned = false;
bool levelOver = false;
bool gameFinished = false;
int gameLevel = 1;
int backgroundLevel = 0;
bool increaseTens = false;
bool increaseHundreds = false;
bool increaseThousands = false;
bool increaseTenThousands = false;
bool increaseHundredThousands = false;
bool increaseMillions = false;
bool highScoreRendering = false;
int TOP_PILLAR_Y_OFFSET = 300;
int PILLAR_SPACE_OFFSET = 325;
int HIGH_SCORE_X_CURRENT_POSITION = HIGH_SCORE_X_STARTING_POSITION;
int SCORE_SCREEN_ONES_SPOT_X_POSITION = SCREEN_WIDTH - 405;
int ONES_SPOT_X_POSITION = SCREEN_WIDTH - 70;
float LEVEL_SPEED = 200;
float FLAME_SPEED = 100;
float OBSTACLE_GRAVITY = 10;
int ARROW_DELAY = 100;
Arrow* arrow;
Character character;
const Uint8* keyState;

/***************************************
* END OF VARIABLE / CONSTANT DECLARATION
****************************************/

Game::Game()
{
	window = nullptr;
	gameState = GameState::TITLE; //Use this because its an enum variable
}

Game::~Game()
{

}

/**************************************************
* Generate a position between the specified values
**************************************************/
int Game::randomYPosition()
{
	int random = (rand() % 230) + 50;
	return random;
}

/**************************************************
 * Generate a position between the specified values
 **************************************************/
int Game::randomXPosition()
{
	int random = (rand() % 150) + 100;
	return random;
}

void Game::mainmenu()
{
    SDL_RenderCopy(renderTarget,menu_tex,NULL,NULL);

    TTF_Init();

    font = TTF_OpenFont("Gaegu-Bold.ttf",1000);
    SDL_Texture *text[4];
    SDL_Color text_color = {255,255,255,255};
    SDL_FreeSurface(menu_surface);
    menu_surface = TTF_RenderText_Solid(font,"NEW GAME",text_color);
    text[0] = SDL_CreateTextureFromSurface(renderTarget,menu_surface);
    SDL_FreeSurface(menu_surface);
    menu_surface = TTF_RenderText_Solid(font,"SEEK HELP",text_color);
    text[1] = SDL_CreateTextureFromSurface(renderTarget,menu_surface);
    SDL_FreeSurface(menu_surface);
    menu_surface = TTF_RenderText_Solid(font,"CREDIT    ",text_color);
    text[2] = SDL_CreateTextureFromSurface(renderTarget,menu_surface);
    menu_surface = TTF_RenderText_Solid(font,"QUIT GAME",text_color);
    text[3] = SDL_CreateTextureFromSurface(renderTarget,menu_surface);

	

    SDL_Rect text_position;
    int i,j;
    for(i=0,j=0;i<4;i++,j+=42)
    {
   	 text_position.x = 430;
   	 text_position.y = 280+j;
   	 text_position.w = 120;
   	 text_position.h = 40;
   	 SDL_RenderCopy(renderTarget,text[i],NULL,&text_position);
    }
    SDL_RenderPresent(renderTarget);
    SDL_Event a;
    bool menu_run = true;
    
    while(menu_run)
    {
   	 while(SDL_PollEvent(&a))
   	 {
   		 switch(a.type)
   		 {
   			 case SDL_QUIT:
   			 {
				 menu_run = false;
				 gameState = GameState::EXIT_FROM_TITLE;
   				 break;
   			 }

   			 case SDL_MOUSEBUTTONDOWN:
   			 {
   				 if(a.motion.x>=430 && a.motion.x<=550 && a.motion.y>=290 && a.motion.y<310)
   				 {
						 menu_run=false;
						 gameState = GameState::PLAY;
	             }
   				 

   				 else if(a.motion.x>=430 && a.motion.x<=550 && a.motion.y>=330 && a.motion.y<350)
   				 {
					 menu_run=false;
					 SDL_RenderClear(renderTarget);
   					 SDL_RenderCopy(renderTarget,help_tex, NULL, NULL);
                     SDL_RenderPresent(renderTarget);
   					 SDL_Event event1;
   					 bool running = true;

   					 while(running)
   					 {
   				  	 while(SDL_PollEvent(&event1))
   				  	 {
   							 if(event1.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
   							 {
								 running=false;
   								 mainmenu();
   							 }
   							  switch(event1.type)
   							 {
   								 case SDL_QUIT:
   								 {  	
										 running=false;
										 gameState = GameState::EXIT_FROM_TITLE;										 
   									     break;
   								 }
						    } 
   					 }		
   					 }
   			      }
   				 else if(a.motion.x>=430 && a.motion.x<=520 && a.motion.y>=370 && a.motion.y<390)
   				  {
					 menu_run=false;
					 SDL_RenderClear(renderTarget);
   					 SDL_RenderCopy(renderTarget,credit_tex, NULL, NULL);
                     SDL_RenderPresent(renderTarget);
   					 SDL_Event event1;
   					 bool running = true;

   					 while(running)
   					 {
   				  	 while(SDL_PollEvent(&event1))
   				  	 {
   							 if(event1.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
   							 {
								 running=false;
   								 mainmenu();
   							 }
   							  switch(event1.type)
   							 {
   								 case SDL_QUIT:
   								 {  	
										 running=false;
										 gameState = GameState::EXIT_FROM_TITLE;										 
   									     break;
   								 }
						    } 
   					 }		
   					 }
   				  }
   				 else if(a.motion.x>=430 && a.motion.x<=550 && a.motion.y>=410 && a.motion.y<435)
   				 {
					 menu_run=false;
					 SDL_RenderClear(renderTarget);
   					 SDL_RenderCopy(renderTarget, quit_tex, NULL, NULL);
					 SDL_RenderPresent(renderTarget);
   					 SDL_Event b;
   					 bool exit_run = true;
   					 while(exit_run)
   					 {
   						 while(SDL_PollEvent(&b))
   						 {
   							 switch(b.type)
   							 {
   								 case SDL_QUIT:
   								 {
										 exit_run=false;
										 gameState = GameState::EXIT_FROM_TITLE;	
   									     break;
   								 }

   								 case SDL_MOUSEBUTTONDOWN:
   								 {
   									 if(b.motion.x>=435 && b.motion.x<=480 && b.motion.y>=380 && b.motion.y<405)
   									 {
											//if pressed YES
										 exit_run=false;
										 gameState = GameState::EXIT_FROM_TITLE;
   									 }
   									 else if(b.motion.x>=538 && b.motion.x<=570 && b.motion.y>=380 && b.motion.y<405)
   									 {
											//if pressed NO
										 exit_run=false;	
   										 mainmenu();
   									 }
										break;
   								 }	
   							 }
   						 }
   					 }
   				 }
					break;
   			 }
				default:
   			    break;
   		 }
   	 }
   }

   
}


void Game::congratulationsScreen()
{
	SDL_Event event;
	SDL_RenderCopy(renderTarget, backgroundBeatGame, &cameraRect, NULL);
	renderScores();
	SDL_RenderPresent(renderTarget);

	while (SDL_PollEvent(&event)) //We use the & as SDL_PollEvent takes an addresss
	{
		switch (event.type) {
		case SDL_QUIT:
			gameState = GameState::EXIT_FROM_TITLE;
			break;

		default:
			break;
		}//switch
	}//while

	keyState = SDL_GetKeyboardState(NULL);


	if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]) //SCANCODE is the best option for different keyboard styles
	{								  //it always gets the proper key
		resetScores();
		setScoreDigitstoGamePosition();
		gameState = GameState::PLAY;
	}
}

void Game::scoreScreen()
{
	SDL_Event event;
	SDL_RenderCopy(renderTarget, backgroundScore, &cameraRect, NULL);
	renderScores();
	SDL_RenderPresent(renderTarget);

	while (SDL_PollEvent(&event)) //We use the & as SDL_PollEvent takes an addresss
	{
		switch (event.type) {
		case SDL_QUIT:
			gameState = GameState::EXIT_FROM_TITLE;
			break;

		default:
			break;
		}//switch
	}//while

	keyState = SDL_GetKeyboardState(NULL);


	if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]) //SCANCODE is the best option for different keyboard styles
	{								  //it always gets the proper key
		resetScores();
		setScoreDigitstoGamePosition();
		gameState = GameState::PLAY;
	}
}

//The initial method that is called when the game starts.
void Game::run()
{
	initSystems();
	while (gameState != GameState::EXIT_FROM_TITLE)
	{
		gameLoop();
	}
}//run

//Initalizes anything we need to run our game.
void Game::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("The Untamed", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	screen = SDL_GetWindowSurface(window);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	cameraRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	backgroundRectFlipped = { SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	loadAllGameTextures();

	SDL_SetRenderDrawColor(renderTarget, 0xFF, 0xFF, 0, 0xFF);

	//DIGIT QUERY TEXTURING//

	SDL_QueryTexture(digits, NULL, NULL, &digitTextureWidth, &digitTextureHeight); //textureWidth / Height get values here.
	digitFrameWidth = digitTextureWidth / 10; //Divide the textureWidth by the number of animation columns
	digitFrameHeight = digitTextureHeight / 1; //Divide the textureHeight by the number of animation rows
	ones = new ScoreDigit(digits, ONES_SPOT_X_POSITION, DIGIT_Y_POSITION);
	tens = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 1, DIGIT_Y_POSITION);
	hundreds = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 2, DIGIT_Y_POSITION);
	thousands = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 3, DIGIT_Y_POSITION);
	tenThousands = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 4, DIGIT_Y_POSITION);
	hundredThousands = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 5, DIGIT_Y_POSITION);
	millions = new ScoreDigit(digits, ONES_SPOT_X_POSITION - digitFrameWidth * 6, DIGIT_Y_POSITION);

	highScoreOnes = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION, DIGIT_Y_POSITION);
	highScoreTens = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 1, DIGIT_Y_POSITION);
	highScoreHundreds = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 2, DIGIT_Y_POSITION);
	highScoreThousands = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 3, DIGIT_Y_POSITION);
	highScoreTenThousands = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 4, DIGIT_Y_POSITION);
	highScoreHundredThousands = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 5, DIGIT_Y_POSITION);
	highScoreMillions = new ScoreDigit(digits, SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 6, DIGIT_Y_POSITION);

	SDL_QueryTexture(highScoreText, NULL, NULL, &highScoreTextureWidth, &highScoreTextureHeight); //textureWidth / Height get values here.
	highScoreRect.x = 20;
	highScoreRect.y = DIGIT_Y_POSITION + 5;
	highScoreRect.w = highScoreTextureWidth;
	highScoreRect.h = highScoreTextureHeight;

	SDL_QueryTexture(collectedGemText, NULL, NULL, &collectedGemTextureWidth, &collectedGemTextureHeight); //textureWidth / Height get values here.
	collectedGemRect.x = SCREEN_WIDTH - 250;
	collectedGemRect.y = DIGIT_Y_POSITION + 50;
	collectedGemRect.w = collectedGemTextureWidth;
	collectedGemRect.h = collectedGemTextureHeight;
	//---------------------//

	SDL_QueryTexture(runningMan, NULL, NULL, &characterTextureWidth, &characterTextureHeight); //textureWidth / Height get values here.
	characterFrameWidth = characterTextureWidth / 5; //Divide the textureWidth by the number of animation columns
	characterFrameHeight = characterTextureHeight / 1; //Divide the textureHeight by the number of animation rows
	character = Character(runningMan, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, characterTextureWidth, characterTextureHeight);

	character.rect.w = characterFrameWidth;
	character.rect.h = characterFrameHeight;
	character.currentPosition.y = SCREEN_HEIGHT - character.currentPosition.h - BOTTOM_SPACE_OFFSET;

}//initSystems

//The main game loop that polls for event occurances.
void Game::gameLoop()
{
	srand(time(0)); //Makes numbers random by seeding a time

	while (gameState == GameState::TITLE)
	{
		mainmenu();
	}

	while (gameState == GameState::PLAY && gameState != GameState::VICTORY)
	{
		processInput();
	}

	if (gameState == GameState::VICTORY)
	{
		congratulationsScreen();
	}

	if (gameState == GameState::SCORE_SCREEN)
	{
		scoreScreen();
	}

	if (gameState == GameState::EXIT_FROM_TITLE)
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderTarget);
		SDL_DestroyTexture(background);
		background = nullptr;
	}

}//gameLoop


void Game::processInput()
{

	levelOver = false;
	prevTime = currTime;
	currTime = SDL_GetTicks();
	deltaTime = (currTime - prevTime) / 1000.0f;

	SDL_Event event;

	while (SDL_PollEvent(&event)) //We use the & as SDL_PollEvent takes an addresss
	{
		switch (event.type) {

			case SDL_QUIT:
			gameState = GameState::EXIT_FROM_TITLE;
			break;

    	default:
	  	break;
		}//switch
	}//while

	keyState = SDL_GetKeyboardState(NULL);

	if ((keyState[SDL_SCANCODE_RIGHT] && !gameFinished) || (keyState[SDL_SCANCODE_D] && !gameFinished)) //SCANCODE is the best option for different keyboard styles
	{															//it always gets the proper key
		character.currentPosition.x += MOVE_RIGHT * deltaTime;
		if(character.currentPosition.x + (character.currentPosition.w) >= SCREEN_WIDTH)
			character.currentPosition.x = SCREEN_WIDTH - character.currentPosition.w;
	}

	if ((keyState[SDL_SCANCODE_LEFT] && !gameFinished) || (keyState[SDL_SCANCODE_A] && !gameFinished))
	{
		character.currentPosition.x += MOVE_LEFT * deltaTime;
		if (character.currentPosition.x < 0)
			character.currentPosition.x = 0;
	}

	if ((keyState[SDL_SCANCODE_UP] && !character.inAir && gameState == GameState::PLAY) ||
		(keyState[SDL_SCANCODE_W] && !character.inAir && gameState == GameState::PLAY))
	{
		character.yVel = MOVE_UP;
		if (character.currentPosition.y <= MAX_HEIGHT)
		{
			character.inAir = true;
		}
	}

	else
	{
		character.inAir = true;
	}

	//Handle Player Jumping Arch
	character.currentPosition.y += character.yVel * deltaTime;
	character.yVel += GRAVITY;

	if (character.currentPosition.y + character.currentPosition.h > SCREEN_HEIGHT - BOTTOM_SPACE_OFFSET)
	{
		character.currentPosition.y = SCREEN_HEIGHT - character.currentPosition.h - BOTTOM_SPACE_OFFSET;
		character.inAir = false;
	}

	randoms = (rand() % 100) + 1;
	randomY = randomYPosition();
	randomX = randomXPosition();

	if (gameLevel == 1 && !ghostTitleScreenBlock)
	{
		levelOne();
	}

	if (gameLevel == 2)
	{
		levelTwo();
	}

	if (gameLevel == 3)
	{
		levelThree();
	}

	if (gameLevel == 4)
	{
		levelFour();
	}

	if (gameLevel == 5)
	{
		levelFive();
	}

	if (gameLevel == 6)
	{
		beatGame();
	}

	increaseMillions = true;

	frameTime += deltaTime; //No matter how fast or slow your computer is, the same updates
							//will occurr across all processors.
	flameFrameTime += deltaTime;
	scoreFrameTime += deltaTime;

	if (frameTime >= characterAnimationTime)
	{
		frameTime = 0; //Use this loop to cycle through animations
		character.rect.x += characterFrameWidth;
		gems.nextFrameAnimation();
	}

	if (scoreFrameTime >= scoreAnimationTime)
	{
		scoreFrameTime = 0;
		ones->nextFrameAnimation();
		currentScoreTimer++;
	}

	if (increaseTens)
	{
		tens->nextFrameAnimation();
		increaseTens = false;
	}

	if (increaseHundreds)
	{
		hundreds->nextFrameAnimation();
		increaseHundreds = false;
	}

	if (increaseThousands)
	{
		thousands->nextFrameAnimation();
		increaseThousands = false;
	}

	if (increaseTenThousands)
	{
		tenThousands->nextFrameAnimation();
		increaseTenThousands = false;
	}

	if (increaseHundredThousands)
	{
		hundredThousands->nextFrameAnimation();
		increaseHundredThousands = false;
	}

	if (increaseMillions)
	{
		millions->nextFrameAnimation();
		increaseMillions = false;
	}

	if (flameFrameTime >= flameAnimationTime)
	{
		flameFrameTime = 0;
		flames.nextFrameAnimation();
	}

	if (character.rect.x >= characterTextureWidth - 1) //USED -1 because the last frame causes flickering
	{
		character.rect.x = 0; //If we advance beyond the last sprite sheet, start back from the first animation.
		gems.setToStartAnimation();
	}

	bool endAnimation = flames.reachedLastAnimation();

	if (endAnimation)
	{
		flames.setToStartAnimation();
		endAnimation = false;
	}

	if (ones->reachedLastAnimation())
	{
		ones->setToStartAnimation();
		tens->shouldBeRendered = true;
		increaseTens = true;
	}

	if (tens->reachedLastAnimation())
	{
		tens->setToStartAnimation();
		hundreds->shouldBeRendered = true;
		increaseHundreds = true;
	}

	if (hundreds->reachedLastAnimation())
	{
		hundreds->setToStartAnimation();
		thousands->shouldBeRendered = true;
		increaseThousands = true;
	}

	if (thousands->reachedLastAnimation())
	{
		thousands->setToStartAnimation();
		tenThousands->shouldBeRendered = true;
		increaseTenThousands = true;
	}

	if (tenThousands->reachedLastAnimation())
	{
		tenThousands->setToStartAnimation();
		hundredThousands->shouldBeRendered = true;
		increaseHundredThousands = true;
	}

	if (hundredThousands->reachedLastAnimation())
	{
		hundredThousands->setToStartAnimation();
		millions->shouldBeRendered = true;
		increaseMillions = true;
	}

	obstacles.update(deltaTime);
	gems.update(deltaTime);
	flames.update(deltaTime);
	SDL_RenderClear(renderTarget);

	backgroundRect.x--;
	backgroundRectFlipped.x--;

	if (backgroundRect.x <= -SCREEN_WIDTH)
	{
		backgroundRect.x = SCREEN_WIDTH;
	}

	if (backgroundRectFlipped.x <= -SCREEN_WIDTH)
	{
		backgroundRectFlipped.x = SCREEN_WIDTH;
	}

	if (!backgroundLevelSet)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	if (backgroundLevelSet && backgroundLevel == 1)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	if (backgroundLevelSet && backgroundLevel == 2)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	if (backgroundLevelSet && backgroundLevel == 3)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	if (backgroundLevelSet && backgroundLevel == 4)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	if (backgroundLevelSet && backgroundLevel == 5)
	{
		SDL_RenderCopy(renderTarget, background, NULL, &backgroundRect);
		SDL_RenderCopy(renderTarget, backgroundFlipped, NULL, &backgroundRectFlipped);
	}

	bool collectedGem = gems.detectCollision(character);

	if (collectedGem)
	{
		increaseThousands = true;
		hundreds->shouldBeRendered = true;
		thousands->shouldBeRendered = true;
		collectedGemTextTimer = 100;
		currentScoreTimer += 1000;
	}

	if (collectedGemTextTimer <= 0)
	{
		collectedGemTextTimer = 0;
	}

	bool flamesGameOver = flames.detectCollision(character);
	bool obstaclesGameOver = obstacles.detectCollision(character);

	if (obstaclesGameOver || flamesGameOver)
	{
		resetGameVariables();
	}

	ghostTitleScreenBlock = false;
	obstacles.draw(renderTarget);
	gems.draw(renderTarget);
	flames.draw(renderTarget);
	obstacles.cleanup();
	gems.cleanup();
	flames.cleanup();

	if (highScoreRendering)
	{
		renderHighScore();
	}

	if (gameState != GameState::SCORE_SCREEN)
	{
		SDL_RenderCopy(renderTarget, character.image, &(character.rect), &(character.currentPosition)); //Copies a portion of the texture to the current rendering target

		if (arrow != nullptr && arrow->shouldBeRendered)
		{
			SDL_RenderCopy(renderTarget, arrow->image, &(arrow->rect), &(arrow->currentPosition)); //Copies a portion of the texture to the current rendering target
		}

		if (collectedGemTextTimer > 0)
		{
			SDL_RenderCopy(renderTarget, collectedGemText, NULL, &(collectedGemRect));
		}
		renderScores();
		SDL_RenderPresent(renderTarget); //Updates the screen with any rendering performed since the previous call
	}

	incrementTimers();
}//processInput

void Game::flickerArrow()
{
	if (arrow != nullptr && !fallingBlockSpawned)
	{
		if (arrowFlickerTimer <= 10)
		{
			arrow->shouldBeRendered = true;
		}
		else
		{
			arrow->shouldBeRendered = false;
		}

		if (arrowFlickerTimer >= 20)
			arrowFlickerTimer = 0;
	}
}


/********************************************
 * Reset all variables to their starting state
 *********************************************/
void Game::resetGameVariables()
{
	if (currentScoreTimer > highScoreTimer)
	{
		setNewHighScore();
	}
	OBSTACLE_GRAVITY = 10;
	ARROW_DELAY = 100;
	highScoreRendering = true;
	setScoreDigitsToScorePosition();
	arrow = nullptr;
	collectedGemTextTimer = 0;
	TOP_PILLAR_Y_OFFSET = 300;
	endGameDelayClock = 100;
	gameFinished = false;
	gemSpawnRate = LEVEL_1_SPAWN_RATE * 2;
	flameSpawnRate = LEVEL_1_SPAWN_RATE * 2;
	fallingBlockSpawnRate = LEVEL_1_SPAWN_RATE * 2;
	backgroundRect.x = 0;
	backgroundRectFlipped.x = SCREEN_WIDTH;
	fallingBlockSpawned = false;
	character.currentPosition.x = 0;
	currentScoreTimer = 0;
	character.currentPosition.y = SCREEN_HEIGHT - character.currentPosition.h - BOTTOM_SPACE_OFFSET;
	scoreFrameTime = 0;
	gameLevel = 1;
	blockDelayTimer = 0;
	flameDelayTimer = 0;
	fallingBlockDelayTimer = 0;
	gemDelayTimer = 0;
	ghostTitleScreenBlock = false;
	frameTime = 0;
	backgroundTimer = 0; //Used to time the switch of the background between levels
	backgroundLevelSet = false;
	gameState = GameState::SCORE_SCREEN;
	LEVEL_SPEED = 200;
	FLAME_SPEED = 200;
	characterAnimationTime = .16;
	backgroundLevel = 0;
	backgroundLevelSet = false;
	obstacles.removeAll();
	flames.removeAll();
	gems.removeAll();
}//resetGameVariables

/******************************************************************
 * Reset all the game scores back to 0 and turn off their rendering
 ******************************************************************/
void Game::resetScores()
{
	SCORE_SCREEN_ONES_SPOT_X_POSITION = SCREEN_WIDTH - 405;
	ones->moveRect.x = 0;
	tens->moveRect.x = 0;
	hundreds->moveRect.x = 0;
	thousands->moveRect.x = 0;
	tenThousands->moveRect.x = 0;
	hundredThousands->moveRect.x = 0;
	millions->moveRect.x = 0;
	ones->shouldBeRendered = false;
	tens->shouldBeRendered = false;
	hundreds->shouldBeRendered = false;
	thousands->shouldBeRendered = false;
	tenThousands->shouldBeRendered = false;
	hundredThousands->shouldBeRendered = false;
	millions->shouldBeRendered = false;
}//resetScores

/***********************************************************
 * Copy all Score Digits with shouldBeRendered to the screen
 ************************************************************/
void Game::renderScores()
{
	SDL_RenderCopy(renderTarget, ones->image, &(ones->moveRect), (&ones->spotRect));

	if (tens->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, tens->image, &(tens->moveRect), (&tens->spotRect));
	}
	if (hundreds->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, hundreds->image, &(hundreds->moveRect), (&hundreds->spotRect));
	}
	if (thousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, thousands->image, &(thousands->moveRect), (&thousands->spotRect));
	}
	if (tenThousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, tenThousands->image, &(tenThousands->moveRect), (&tenThousands->spotRect));
	}
	if (hundredThousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, hundredThousands->image, &(hundredThousands->moveRect), (&hundredThousands->spotRect));
	}
	if (millions->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, millions->image, &(millions->moveRect), (&millions->spotRect));
	}
}//renderScores

void Game::setNewHighScore()
{
	highScoreTimer = currentScoreTimer;
	currentScoreTimer = 0;

	if (millions->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 6)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreMillions->shouldBeRendered = true;
		highScoreMillions->spotRect = millions->spotRect;
		highScoreMillions->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 6;
		highScoreMillions->moveRect = millions->moveRect;
	}

	if (hundredThousands->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 5)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreHundredThousands->shouldBeRendered = true;
		highScoreHundredThousands->spotRect = hundredThousands->spotRect;
		highScoreHundredThousands->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 5;
		highScoreHundredThousands->moveRect = hundredThousands->moveRect;
	}

	if (tenThousands->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 4)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreTenThousands->shouldBeRendered = true;
		highScoreTenThousands->spotRect = tenThousands->spotRect;
		highScoreTenThousands->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 4;
		highScoreTenThousands->moveRect = tenThousands->moveRect;
	}

	if (thousands->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 3)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreThousands->shouldBeRendered = true;
		highScoreThousands->spotRect = thousands->spotRect;
		highScoreThousands->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 3;
		highScoreThousands->moveRect = thousands->moveRect;
	}

	if (hundreds->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 2)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreHundreds->shouldBeRendered = true;
		highScoreHundreds->spotRect = hundreds->spotRect;
		highScoreHundreds->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 2;
		highScoreHundreds->moveRect = hundreds->moveRect;
	}

	if (tens->shouldBeRendered)
	{
		while (HIGH_SCORE_X_CURRENT_POSITION < HIGH_SCORE_X_STARTING_POSITION + digitFrameWidth * 1)
		{
			HIGH_SCORE_X_CURRENT_POSITION += digitFrameWidth;
		}
		highScoreTens->shouldBeRendered = true;
		highScoreTens->spotRect = tens->spotRect;
		highScoreTens->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION - digitFrameWidth * 1;
		highScoreTens->moveRect = tens->moveRect;
	}

	highScoreOnes->shouldBeRendered = true;
	highScoreOnes->spotRect = ones->spotRect;
	highScoreOnes->spotRect.x = HIGH_SCORE_X_CURRENT_POSITION;
	highScoreOnes->moveRect = ones->moveRect;

}

void Game::renderHighScore()
{
	SDL_RenderCopy(renderTarget, highScoreText, NULL, &(highScoreRect));

	SDL_RenderCopy(renderTarget, highScoreOnes->image, &(highScoreOnes->moveRect), (&highScoreOnes->spotRect));

	if (highScoreTens->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, highScoreTens->image, &(highScoreTens->moveRect), (&highScoreTens->spotRect));
	}
	if (highScoreHundreds->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, highScoreHundreds->image, &(highScoreHundreds->moveRect), (&highScoreHundreds->spotRect));
	}
	if (highScoreThousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, highScoreThousands->image, &(highScoreThousands->moveRect), (&highScoreThousands->spotRect));
	}
	if (highScoreTenThousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, highScoreTenThousands->image, &(highScoreTenThousands->moveRect), (&highScoreTenThousands->spotRect));
	}
	if (highScoreHundredThousands->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, highScoreHundredThousands->image, &(highScoreHundredThousands->moveRect), (&highScoreHundredThousands->spotRect));
	}
	if (highScoreMillions->shouldBeRendered)
	{
		SDL_RenderCopy(renderTarget, millions->image, &(millions->moveRect), (&millions->spotRect));
	}

}

/****************************************************************************
* Load a single .bmp fie from the folder where all .cpp / .h files are stored
*****************************************************************************/
SDL_Texture* Game::loadSingleTexture(string filePath, SDL_Renderer* renderTarget)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = SDL_LoadBMP(filePath.c_str()); //Loads surface from your filePath on your computer.
	if (surface == NULL)
	{
		cout << "Cannot find image!" << endl;
	}
	else
	{
		//This sets a mask for the given colour so you don't see it.
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 200, 55, 150));
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
		{
			cout << "Texture is null!" << endl;
		}
	}

	SDL_FreeSurface(surface);
	return texture;
}//loadSingleTexture

/**************************************************************
* Load all the game textures so they can be displayed on screen
**************************************************************/
void Game::loadAllGameTextures()
{

	//title = loadSingleTexture("titleScreen.bmp", renderTarget);
	menu_tex = loadSingleTexture("menu.bmp",renderTarget);
	credit_tex = loadSingleTexture("credit.bmp",renderTarget);
	help_tex = loadSingleTexture("help.bmp", renderTarget);
	quit_tex = loadSingleTexture("quitgame.bmp",renderTarget);
	background = loadSingleTexture("background.bmp", renderTarget);
	backgroundFlipped = loadSingleTexture("background.bmp", renderTarget);
	backgroundScore = loadSingleTexture("backgroundScore.bmp", renderTarget);
	backgroundBeatGame = loadSingleTexture("backgroundBeatGame.bmp", renderTarget);

	blueGem = loadSingleTexture("bluegem.bmp", renderTarget);
	greenGem = loadSingleTexture("greengem.bmp", renderTarget);
	yellowGem = loadSingleTexture("yellowgem.bmp", renderTarget);
	redGem = loadSingleTexture("redgem.bmp", renderTarget);
	blackGem = loadSingleTexture("blackgem.bmp", renderTarget);

	brownBlock = loadSingleTexture("brownblock.bmp", renderTarget); // level 1
	steelBlock = loadSingleTexture("steelblock.bmp", renderTarget); // level 2
	yellowBlock = loadSingleTexture("yellowblock.bmp", renderTarget); // level 3
	diamondBlock = loadSingleTexture("diamondblock.bmp", renderTarget); // level 4
	woodenBlock = loadSingleTexture("woodenblock.bmp", renderTarget); // level 5

	brownBlockBroken = loadSingleTexture("brownblockbroken.bmp", renderTarget); // level 1
	steelBlockBroken = loadSingleTexture("steelblockbroken.bmp", renderTarget); // level 2
	yellowBlockBroken = loadSingleTexture("yellowblockbroken.bmp", renderTarget); // level 3
	diamondBlockBroken = loadSingleTexture("diamondblockbroken.bmp", renderTarget); // level 4
	woodenBlockBroken = loadSingleTexture("woodenblockbroken.bmp", renderTarget); // level 5

	brownPillar = loadSingleTexture("brownpillar.bmp", renderTarget); // level 1
	steelPillar = loadSingleTexture("steelpillar.bmp", renderTarget); // level 2
	yellowPillar = loadSingleTexture("yellowpillar.bmp", renderTarget); // level 3
	diamondPillar = loadSingleTexture("diamondpillar.bmp", renderTarget); // level 4
	woodenPillar = loadSingleTexture("woodenpillar.bmp", renderTarget); // level 5

	runningMan = loadSingleTexture("character.bmp", renderTarget);
	redFlame = loadSingleTexture("redFlame.bmp", renderTarget);
	blueFlame = loadSingleTexture("blueFlame.bmp", renderTarget);
	purpleFlame = loadSingleTexture("purpleFlame.bmp", renderTarget);

	highScoreText = loadSingleTexture("highScore.bmp", renderTarget);
	collectedGemText = loadSingleTexture("collectedGem.bmp", renderTarget);
	digits = loadSingleTexture("digits.bmp", renderTarget);
	redArrow = loadSingleTexture("redArrow.bmp", renderTarget);

}//loadAllGameTextures

/**************************************************
* Set the Score Digits back to the top right corner
***************************************************/
void Game::setScoreDigitstoGamePosition()
{
	ones->spotRect.x = ONES_SPOT_X_POSITION;
	ones->spotRect.y = DIGIT_Y_POSITION;
	tens->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 1;
	tens->spotRect.y = DIGIT_Y_POSITION;
	hundreds->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 2;
	hundreds->spotRect.y = DIGIT_Y_POSITION;
	thousands->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 3;
	thousands->spotRect.y = DIGIT_Y_POSITION;
	tenThousands->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 4;
	tenThousands->spotRect.y = DIGIT_Y_POSITION;
	hundredThousands->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 5;
	hundredThousands->spotRect.y = DIGIT_Y_POSITION;
	millions->spotRect.x = ONES_SPOT_X_POSITION - digitFrameWidth * 6;
	millions->spotRect.y = DIGIT_Y_POSITION;
}//setScoreDigitstoGamePosition

/**************************************************************
* Set the Score Digits to the center of the screen depending
  on how many Score Digits are currently rendered to the screen
***************************************************************/
void Game::setScoreDigitsToScorePosition()
{
	bool setScorePosition = false;

	if (millions->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
	}

	if (hundredThousands->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= digitFrameWidth / 2;
	}

	if (tenThousands->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= digitFrameWidth;
	}

	if (thousands->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= digitFrameWidth;
	}

	if (hundreds->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= (digitFrameWidth + (digitFrameWidth / 2));
	}

	if (tens->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= (digitFrameWidth * 2);
	}

	if (ones->shouldBeRendered && !setScorePosition)
	{
		setScorePosition = true;
		SCORE_SCREEN_ONES_SPOT_X_POSITION -= ((digitFrameWidth * 2) + (digitFrameWidth / 2));
	}

	ones->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION;
	ones->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	tens->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 1;
	tens->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	hundreds->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 2;
	hundreds->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	thousands->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 3;
	thousands->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	tenThousands->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 4;
	tenThousands->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	hundredThousands->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 5;
	hundredThousands->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
	millions->spotRect.x = SCORE_SCREEN_ONES_SPOT_X_POSITION - digitFrameWidth * 6;
	millions->spotRect.y = SCORE_SCREEN_DIGIT_Y_POSITION;
}//setScoreDigitsToScorePosition

 /********************************************************************
 * Timers, variables, and offsets used at the first level of the game
 ********************************************************************/
void Game::levelOne()
{
	levelOver = false;

		if (!backgroundLevelSet && backgroundLevel == 0)
		{
			backgroundLevelSet = true;
			backgroundLevel++;
			backgroundTimer = currTime + BACKGROUND_APPEAR_TIMER;
		}

		if (backgroundTimer <= currTime)
		{
			backgroundLevelSet = false;
		}

		if (blockDelayTimer != 0 && blockDelayTimer % LEVEL_1_INTERVAL == 0)
		{
			levelOver = true;
			blockDelayTimer = 0;
			gemDelayTimer = 0;
			flameDelayTimer = 0;
			fallingBlockDelayTimer = 0;
			gameLevel++;
		}

		if (blockDelayTimer % LEVEL_1_SPAWN_RATE == 0 && !levelOver)
		{
			canSpawnBlock = true;
		}

		if (canSpawnBlock)
		{
			if (randoms < 50)
			{
				canSpawnBlock = false;
				SDL_QueryTexture(brownBlock, NULL, NULL, &blockTextureWidth, &blockTextureHeight);
				blockFrameWidth = blockTextureWidth / 1;
				blockFrameHeight = blockTextureHeight / 1;
				Block* block = new Block(brownBlock, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
				obstacles.add(block);
			}

			else
			{
				canSpawnBlock = false;
				SDL_QueryTexture(brownPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
				pillarFrameWidth = pillarTextureWidth / 1;
				pillarFrameHeight = pillarTextureHeight / 1;
				BottomPillar* bottomPillar = new BottomPillar(brownPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarFrameWidth, pillarFrameHeight);
				obstacles.add(bottomPillar);

				SDL_QueryTexture(brownPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
				pillarFrameWidth = pillarTextureWidth / 1;
				pillarFrameHeight = pillarTextureHeight / 1;
				TopPillar* topPillar = new TopPillar(brownPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarFrameWidth, pillarFrameHeight);
				obstacles.add(topPillar);
			}
		}

		if (gemDelayTimer != 0 && gemDelayTimer % gemSpawnRate == 0 && !levelOver)
		{
			gemDelayTimer = 0;
			SDL_QueryTexture(blueGem, NULL, NULL, &gemTextureWidth, &gemTextureHeight); //textureWidth / Height get values here.
			gemFrameWidth = gemTextureWidth / 8; //Divide the textureWidth by the number of animation columns
			gemFrameHeight = gemTextureHeight / 1; //Divide the textureHeight by the number of animation rows
			Gem* gem = new Gem(blueGem, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gemFrameWidth, gemFrameHeight);
			gems.add(gem);
		}

		if (fallingBlockDelayTimer + ARROW_DELAY >= fallingBlockSpawnRate && !fallingBlockSpawned)
		{
			SDL_QueryTexture(redArrow, NULL, NULL, &arrowTextureWidth, &arrowTextureHeight); //textureWidth / Height get values here.
			arrowFrameWidth = arrowTextureWidth / 1; //Divide the textureWidth by the number of animation columns
			arrowFrameHeight = arrowTextureHeight / 1; //Divide the textureHeight by the number of animation rows
			arrow = new Arrow(redArrow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, arrowFrameWidth, arrowFrameHeight, character);
		}

		if (fallingBlockDelayTimer != 0 && fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
		{
			fallingBlockSpawned = true;
			arrow->shouldBeRendered = false;
			SDL_QueryTexture(brownBlockBroken, NULL, NULL, &blockTextureWidth, &blockTextureHeight); //textureWidth / Height get values here.
			blockFrameWidth = blockTextureWidth / 1; //Divide the textureWidth by the number of animation columns
			blockFrameHeight = blockTextureHeight / 1; //Divide the textureHeight by the number of animation rows
			FallingBlock* fallingBlock = new FallingBlock(brownBlockBroken, arrow->currentPosition.x, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
			obstacles.add(fallingBlock);
			arrow = nullptr;
		}

		if (flameDelayTimer != 0 && flameDelayTimer % flameSpawnRate == 0 && !levelOver)
		{
			SDL_QueryTexture(redFlame, NULL, NULL, &flameTextureWidth, &flameTextureHeight); //textureWidth / Height get values here.
			flameFrameWidth = flameTextureWidth / 6; //Divide the textureWidth by the number of animation columns
			flameFrameHeight = flameTextureHeight / 1; //Divide the textureHeight by the number of animation rows
			Flame* flame = new Flame(redFlame, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, flameFrameWidth, flameFrameHeight);
			flames.add(flame);
		}

		if (fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
		{
			fallingBlockSpawned = false;
			fallingBlockDelayTimer = 0;
		}

		flickerArrow();
}

/********************************************************************
* Timers, variables, and offsets used at the second level of the game
********************************************************************/
void Game::levelTwo()
{
	levelOver = false;
	OBSTACLE_GRAVITY = 12;
	ARROW_DELAY = 90;
	LEVEL_SPEED = 250;
	FLAME_SPEED += 25;
	PILLAR_SPACE_OFFSET = 325;
	TOP_PILLAR_Y_OFFSET = 280;
	gemSpawnRate = LEVEL_2_SPAWN_RATE * 2;
	flameSpawnRate = LEVEL_2_SPAWN_RATE * 2;
	fallingBlockSpawnRate = LEVEL_2_SPAWN_RATE * 2;
	characterAnimationTime = .15;

	if (!backgroundLevelSet && backgroundLevel == 1)
	{
		backgroundLevelSet = true;
		backgroundLevel++;
		backgroundTimer = currTime + BACKGROUND_APPEAR_TIMER;
	}

	if (backgroundTimer <= currTime)
	{
		backgroundLevelSet = false;
	}

	if (blockDelayTimer != 0 && blockDelayTimer % LEVEL_2_INTERVAL == 0)
	{
		blockDelayTimer = 0;
		levelOver = true;
		gemDelayTimer = 0;
		flameDelayTimer = 0;
		fallingBlockDelayTimer = 0;
		gameLevel++;
	}

	if (blockDelayTimer % LEVEL_2_SPAWN_RATE == 0 && !levelOver)
	{
		canSpawnBlock = true;
	}

	if (canSpawnBlock)
	{
		if (randoms < 50)
		{
			canSpawnBlock = false;
			SDL_QueryTexture(steelBlock, NULL, NULL, &blockTextureWidth, &blockTextureHeight);
			blockFrameWidth = blockTextureWidth / 1;
			blockFrameHeight = blockTextureHeight / 1;
			Block* block = new Block(steelBlock, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, blockTextureWidth, blockTextureHeight);
			obstacles.add(block);
		}

		else
		{
			canSpawnBlock = false;
			SDL_QueryTexture(steelPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			BottomPillar* bottomPillar = new BottomPillar(steelPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarFrameWidth, pillarFrameHeight);
			obstacles.add(bottomPillar);

			SDL_QueryTexture(steelPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			TopPillar* topPillar = new TopPillar(steelPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarFrameWidth, pillarFrameHeight);
			obstacles.add(topPillar);
		}
	}

	if (gemDelayTimer != 0 && gemDelayTimer % gemSpawnRate == 0 && !levelOver)
	{
		gemDelayTimer = 0;
		SDL_QueryTexture(greenGem, NULL, NULL, &gemTextureWidth, &gemTextureHeight); //textureWidth / Height get values here.
		gemFrameWidth = gemTextureWidth / 8; //Divide the textureWidth by the number of animation columns
		gemFrameHeight = gemTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Gem* gem = new Gem(greenGem, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gemFrameWidth, gemFrameHeight);
		gem->currentPosition.x += randomX;
		gems.add(gem);
	}

	if (fallingBlockDelayTimer + ARROW_DELAY >= fallingBlockSpawnRate && !fallingBlockSpawned)
	{
		SDL_QueryTexture(redArrow, NULL, NULL, &arrowTextureWidth, &arrowTextureHeight); //textureWidth / Height get values here.
		arrowFrameWidth = arrowTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		arrowFrameHeight = arrowTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		arrow = new Arrow(redArrow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, arrowFrameWidth, arrowFrameHeight, character);
	}

	if (fallingBlockDelayTimer != 0 && fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = true;
		arrow->shouldBeRendered = false;
		SDL_QueryTexture(steelBlockBroken, NULL, NULL, &blockTextureWidth, &blockTextureHeight); //textureWidth / Height get values here.
		blockFrameWidth = blockTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		blockFrameHeight = blockTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		FallingBlock* fallingBlock = new FallingBlock(steelBlockBroken, arrow->currentPosition.x, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
		obstacles.add(fallingBlock);
		arrow = nullptr;
	}

	if (flameDelayTimer != 0 && flameDelayTimer % flameSpawnRate == 0 && !levelOver)
	{
		SDL_QueryTexture(redFlame, NULL, NULL, &flameTextureWidth, &flameTextureHeight); //textureWidth / Height get values here.
		flameFrameWidth = flameTextureWidth / 6; //Divide the textureWidth by the number of animation columns
    	flameFrameHeight = flameTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Flame* flame = new Flame(redFlame, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, flameFrameWidth, flameFrameHeight);
		flames.add(flame);
	}

	if (fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = false;
		fallingBlockDelayTimer = 0;
	}

	flickerArrow();
}

/********************************************************************
* Timers, variables, and offsets used at the third level of the game
********************************************************************/
void Game::levelThree()
{
	levelOver = false;
	OBSTACLE_GRAVITY = 14;
	ARROW_DELAY = 70;
	LEVEL_SPEED = 300;
	FLAME_SPEED += 25;
	TOP_PILLAR_Y_OFFSET = 260;
	gemSpawnRate = LEVEL_3_SPAWN_RATE * 2;
	flameSpawnRate = LEVEL_3_SPAWN_RATE * 2;
	fallingBlockSpawnRate = LEVEL_3_SPAWN_RATE * 2;
	characterAnimationTime = .14;

	if (!backgroundLevelSet && backgroundLevel == 2)
	{
		backgroundLevelSet = true;
		backgroundLevel++;
		backgroundTimer = currTime + BACKGROUND_APPEAR_TIMER;
	}

	if (backgroundTimer <= currTime)
	{
		backgroundLevelSet = false;
	}

	if (blockDelayTimer != 0 && blockDelayTimer % LEVEL_3_INTERVAL == 0)
	{
		blockDelayTimer = 0;
		levelOver = true;
		gemDelayTimer = 0;
		flameDelayTimer = 0;
		fallingBlockDelayTimer = 0;
		gameLevel++;
	}

	if (blockDelayTimer % LEVEL_3_SPAWN_RATE == 0 && !levelOver)
	{
		canSpawnBlock = true;
	}

	if (canSpawnBlock)
	{
		if (randoms < 50)
		{
			canSpawnBlock = false;
			SDL_QueryTexture(steelBlock, NULL, NULL, &blockTextureWidth, &blockTextureHeight);
			blockFrameWidth = blockTextureWidth / 1;
			blockFrameHeight = blockTextureHeight / 1;
			Block* block = new Block(steelBlock, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, blockTextureWidth, blockTextureHeight);
			obstacles.add(block);
		}

		else
		{
			canSpawnBlock = false;
			SDL_QueryTexture(steelPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			BottomPillar* bottomPillar = new BottomPillar(steelPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			bottomPillar->rect.w = pillarFrameWidth;
			bottomPillar->rect.h = pillarFrameHeight;
			obstacles.add(bottomPillar);

			SDL_QueryTexture(steelPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			TopPillar* topPillar = new TopPillar(steelPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			topPillar->rect.w = pillarFrameWidth;
			topPillar->rect.h = pillarFrameHeight;
			obstacles.add(topPillar);
		}
	}


	if (gemDelayTimer != 0 && gemDelayTimer % gemSpawnRate == 0 && !levelOver)
	{
		gemDelayTimer = 0;
		SDL_QueryTexture(yellowGem, NULL, NULL, &gemTextureWidth, &gemTextureHeight); //textureWidth / Height get values here.
		gemFrameWidth = gemTextureWidth / 8; //Divide the textureWidth by the number of animation columns
		gemFrameHeight = gemTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Gem* gem = new Gem(yellowGem, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gemFrameWidth, gemFrameHeight);
		gems.add(gem);
	}

	if (fallingBlockDelayTimer + ARROW_DELAY >= fallingBlockSpawnRate && !fallingBlockSpawned)
	{
		SDL_QueryTexture(redArrow, NULL, NULL, &arrowTextureWidth, &arrowTextureHeight); //textureWidth / Height get values here.
		arrowFrameWidth = arrowTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		arrowFrameHeight = arrowTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		arrow = new Arrow(redArrow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, arrowFrameWidth, arrowFrameHeight, character);
	}

	if (fallingBlockDelayTimer != 0 && fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = true;
		arrow->shouldBeRendered = false;
		SDL_QueryTexture(woodenBlockBroken, NULL, NULL, &blockTextureWidth, &blockTextureHeight); //textureWidth / Height get values here.
		blockFrameWidth = blockTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		blockFrameHeight = blockTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		FallingBlock* fallingBlock = new FallingBlock(steelBlockBroken, arrow->currentPosition.x, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
		obstacles.add(fallingBlock);
		arrow = nullptr;
	}

	if (flameDelayTimer != 0 && flameDelayTimer % flameSpawnRate == 0 && !levelOver)
	{
		SDL_QueryTexture(redFlame, NULL, NULL, &flameTextureWidth, &flameTextureHeight); //textureWidth / Height get values here.
		flameFrameWidth = flameTextureWidth / 6; //Divide the textureWidth by the number of animation columns
		flameFrameHeight = flameTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Flame* flame = new Flame(redFlame, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, flameFrameWidth, flameFrameHeight);
		flames.add(flame);
	}

	if (fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = false;
		fallingBlockDelayTimer = 0;
	}

	flickerArrow();
}

/********************************************************************
* Timers, variables, and offsets used at the fourth level of the game
********************************************************************/
void Game::levelFour()
{
	levelOver = false;
	OBSTACLE_GRAVITY = 16;
	ARROW_DELAY = 50;
	LEVEL_SPEED = 350;
	FLAME_SPEED += 25;
	TOP_PILLAR_Y_OFFSET = 240;
	gemSpawnRate = LEVEL_4_SPAWN_RATE * 2;
	flameSpawnRate = LEVEL_4_SPAWN_RATE * 2;
	fallingBlockSpawnRate = LEVEL_4_SPAWN_RATE * 3;
	characterAnimationTime = .13;

	if (!backgroundLevelSet && backgroundLevel == 3)
	{
		backgroundLevelSet = true;
		backgroundLevel++;
		backgroundTimer = currTime + BACKGROUND_APPEAR_TIMER;
	}

	if (backgroundTimer <= currTime)
	{
		backgroundLevelSet = false;
	}

	if (blockDelayTimer != 0 && blockDelayTimer % LEVEL_4_INTERVAL == 0)
	{
		blockDelayTimer = 0;
		levelOver = true;
		gemDelayTimer = 0;
		flameDelayTimer = 0;
		fallingBlockDelayTimer = 0;
		gameLevel++;
	}

	if (blockDelayTimer % LEVEL_4_SPAWN_RATE == 0 && !levelOver)
	{
		canSpawnBlock = true;
	}

	if (canSpawnBlock)
	{
		if (randoms < 50)
		{
			canSpawnBlock = false;
			SDL_QueryTexture(diamondBlock, NULL, NULL, &blockTextureWidth, &blockTextureHeight);
			blockFrameWidth = blockTextureWidth / 1;
			blockFrameHeight = blockTextureHeight / 1;
			Block* block = new Block(diamondBlock, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, blockTextureWidth, blockTextureHeight);
			obstacles.add(block);
		}

		else
		{
			canSpawnBlock = false;
			SDL_QueryTexture(diamondPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			BottomPillar* bottomPillar = new BottomPillar(diamondPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			bottomPillar->rect.w = pillarFrameWidth;
			bottomPillar->rect.h = pillarFrameHeight;
			obstacles.add(bottomPillar);

			SDL_QueryTexture(diamondPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			TopPillar* topPillar = new TopPillar(diamondPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			topPillar->rect.w = pillarFrameWidth;
			topPillar->rect.h = pillarFrameHeight;
			obstacles.add(topPillar);
		}
	}

	if (gemDelayTimer != 0 && gemDelayTimer % gemSpawnRate == 0 && !levelOver)
	{
		gemDelayTimer = 0;
		SDL_QueryTexture(redGem, NULL, NULL, &gemTextureWidth, &gemTextureHeight); //textureWidth / Height get values here.
		gemFrameWidth = gemTextureWidth / 8; //Divide the textureWidth by the number of animation columns
		gemFrameHeight = gemTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Gem* gem = new Gem(redGem, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gemFrameWidth, gemFrameHeight);
		gems.add(gem);
	}

	if (fallingBlockDelayTimer + ARROW_DELAY  >= fallingBlockSpawnRate && !fallingBlockSpawned)
	{
		SDL_QueryTexture(redArrow, NULL, NULL, &arrowTextureWidth, &arrowTextureHeight); //textureWidth / Height get values here.
		arrowFrameWidth = arrowTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		arrowFrameHeight = arrowTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		arrow = new Arrow(redArrow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, arrowFrameWidth, arrowFrameHeight, character);
	}

	if (fallingBlockDelayTimer != 0 && fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = true;
		arrow->shouldBeRendered = false;
		SDL_QueryTexture(diamondBlockBroken, NULL, NULL, &blockTextureWidth, &blockTextureHeight); //textureWidth / Height get values here.
		blockFrameWidth = blockTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		blockFrameHeight = blockTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		FallingBlock* fallingBlock = new FallingBlock(diamondBlockBroken, arrow->currentPosition.x, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
		obstacles.add(fallingBlock);
		arrow = nullptr;
	}

	if (flameDelayTimer != 0 && flameDelayTimer % flameSpawnRate == 0 && !levelOver)
	{
		SDL_QueryTexture(blueFlame, NULL, NULL, &flameTextureWidth, &flameTextureHeight); //textureWidth / Height get values here.
		flameFrameWidth = flameTextureWidth / 6; //Divide the textureWidth by the number of animation columns
		flameFrameHeight = flameTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Flame* flame = new Flame(blueFlame, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, flameFrameWidth, flameFrameHeight);
		flames.add(flame);
	}

	if (fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = false;
		fallingBlockDelayTimer = 0;
	}

	flickerArrow();
}

/********************************************************************
 * Timers, variables, and offsets used at the fifth level of the game
 ********************************************************************/
void Game::levelFive()
{
	levelOver = false;
	OBSTACLE_GRAVITY = 18;
	ARROW_DELAY = 40;
	LEVEL_SPEED = 400;
	FLAME_SPEED += 25;
	TOP_PILLAR_Y_OFFSET = 220;
	gemSpawnRate = LEVEL_5_SPAWN_RATE * 2;
	flameSpawnRate = LEVEL_5_SPAWN_RATE * 2;
	fallingBlockSpawnRate = LEVEL_5_SPAWN_RATE * 2;
	characterAnimationTime = .12;

	if (!backgroundLevelSet && backgroundLevel == 4)
	{
		backgroundLevelSet = true;
		backgroundLevel++;
		backgroundTimer = currTime + BACKGROUND_APPEAR_TIMER;
	}

	if (backgroundTimer <= currTime)
	{
		backgroundLevelSet = false;
	}

	if (blockDelayTimer != 0 && blockDelayTimer % LEVEL_5_INTERVAL == 0)
	{
		blockDelayTimer = 0;
		levelOver = true;
		gemDelayTimer = 0;
		flameDelayTimer = 0;
		fallingBlockDelayTimer = 0;
		gameLevel++;
	}

	if (blockDelayTimer % LEVEL_5_SPAWN_RATE == 0 && !levelOver)
	{
		canSpawnBlock = true;
	}

	if (canSpawnBlock)
	{
		if (randoms < 50)
		{
			canSpawnBlock = false;
			SDL_QueryTexture(woodenBlock, NULL, NULL, &blockTextureWidth, &blockTextureHeight);
			blockFrameWidth = blockTextureWidth / 1;
			blockFrameHeight = blockTextureHeight / 1;
			Block* block = new Block(woodenBlock, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, blockTextureWidth, blockTextureHeight);
			obstacles.add(block);
		}

		else
		{
			canSpawnBlock = false;
			SDL_QueryTexture(woodenPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			BottomPillar* bottomPillar = new BottomPillar(woodenPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			bottomPillar->rect.w = pillarFrameWidth;
			bottomPillar->rect.h = pillarFrameHeight;
			obstacles.add(bottomPillar);

			SDL_QueryTexture(woodenPillar, NULL, NULL, &pillarTextureWidth, &pillarTextureHeight);
			pillarFrameWidth = pillarTextureWidth / 1;
			pillarFrameHeight = pillarTextureHeight / 1;
			TopPillar* topPillar = new TopPillar(woodenPillar, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, pillarTextureWidth, pillarTextureHeight);
			topPillar->rect.w = pillarFrameWidth;
			topPillar->rect.h = pillarFrameHeight;
			obstacles.add(topPillar);

		}
	}

	if (gemDelayTimer != 0 && gemDelayTimer % gemSpawnRate == 0 && !levelOver)
	{
		gemDelayTimer = 0;
		SDL_QueryTexture(blackGem, NULL, NULL, &gemTextureWidth, &gemTextureHeight); //textureWidth / Height get values here.
		gemFrameWidth = gemTextureWidth / 8; //Divide the textureWidth by the number of animation columns
		gemFrameHeight = gemTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Gem* gem = new Gem(blackGem, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, gemFrameWidth, gemFrameHeight);
		gems.add(gem);
	}

	if (fallingBlockDelayTimer + ARROW_DELAY >= fallingBlockSpawnRate && !fallingBlockSpawned)
	{
		SDL_QueryTexture(redArrow, NULL, NULL, &arrowTextureWidth, &arrowTextureHeight); //textureWidth / Height get values here.
		arrowFrameWidth = arrowTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		arrowFrameHeight = arrowTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		arrow = new Arrow(redArrow, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, arrowFrameWidth, arrowFrameHeight, character);
	}

	if (fallingBlockDelayTimer != 0 && fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = true;
		arrow->shouldBeRendered = false;
		SDL_QueryTexture(woodenBlockBroken, NULL, NULL, &blockTextureWidth, &blockTextureHeight); //textureWidth / Height get values here.
		blockFrameWidth = blockTextureWidth / 1; //Divide the textureWidth by the number of animation columns
		blockFrameHeight = blockTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		FallingBlock* fallingBlock = new FallingBlock(woodenBlockBroken, arrow->currentPosition.x, SCREEN_HEIGHT / 2, blockFrameWidth, blockFrameHeight);
		obstacles.add(fallingBlock);
		arrow = nullptr;
	}

	if (flameDelayTimer != 0 && flameDelayTimer % flameSpawnRate == 0 && !levelOver)
	{
		SDL_QueryTexture(purpleFlame, NULL, NULL, &flameTextureWidth, &flameTextureHeight); //textureWidth / Height get values here.
		flameFrameWidth = flameTextureWidth / 6; //Divide the textureWidth by the number of animation columns
		flameFrameHeight = flameTextureHeight / 1; //Divide the textureHeight by the number of animation rows
		Flame* flame = new Flame(purpleFlame, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, flameFrameWidth, flameFrameHeight);
		flames.add(flame);
	}

	if (fallingBlockDelayTimer % fallingBlockSpawnRate == 0 && !levelOver)
	{
		fallingBlockSpawned = false;
		fallingBlockDelayTimer = 0;
	}

	flickerArrow();

}

void Game::beatGame()
{
	endGameDelayClock--;

	if (endGameDelayClock <= 0)
	{
		character.currentPosition.x += END_GAME_MOVE_RIGHT * deltaTime;
		gameFinished = true;
	}

	if (character.currentPosition.x >= SCREEN_WIDTH - 50)
	{
		resetGameVariables();
		SDL_RenderCopy(renderTarget, backgroundBeatGame, NULL, NULL);
		gameState = GameState::VICTORY;
	}

}

void Game::incrementTimers()
{
	blockDelayTimer++;
	gemDelayTimer++;
	flameDelayTimer++;
	fallingBlockDelayTimer++;
	arrowFlickerTimer++;
	collectedGemTextTimer--;
}
