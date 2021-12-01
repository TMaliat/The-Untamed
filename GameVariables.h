#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>

extern float const MOVE_LEFT;
extern float const MOVE_RIGHT;
extern float const MOVE_UP;
extern float const MOVE_DOWN;
extern float const GRAVITY;
extern float const MAX_HEIGHT;
extern float const BOTTOM_SPACE_OFFSET;
extern int const BOTTOM_PILLAR_TOP_OFFSET;
extern int const TOP_PILLAR_BOTTOM_OFFSET;
extern int const FLAME_LEFT_OFFSET;
extern float OBSTACLE_GRAVITY;
extern float LEVEL_SPEED;
extern float FLAME_SPEED;
extern int const SCREEN_WIDTH;
extern int const SCREEN_HEIGHT;
extern int const PILLAR_HEIGHT_OFFSET;
extern int const BLOCK_Y_POS;
extern int const PILLAR_Y_POS;
extern int TOP_PILLAR_Y_OFFSET;
extern int const FALLING_BLOCK_START_X;
extern int gemFrameWidth;
extern int gemTextureWidth;
extern int flameFrameWidth;
extern int flameTextureWidth;
extern int digitFrameWidth;
extern int digitFrameHeight;
extern int digitTextureWidth;
extern int scoreTime;
extern int randomY;
extern int randomX;