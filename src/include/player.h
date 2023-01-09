#pragma once

#include "paddle.h"

#define HUMAN_SPEED     1
#define AI_EASY_SPEED   0.5
#define AI_MEDIUM_SPEED 1.1
#define AI_HARD_SPEED   3

typedef enum {
  NONE,
  UP,
  DOWN
} Direction;

typedef enum {
	BUTTON_1 = 1,
  BUTTON_2 = 2,
  BUTTON_3 = 3,
  BUTTON_4 = 4
} Button;

typedef enum {
  HUMAN,
  AI_EASY,
  AI_MEDIUM,
  AI_HARD
} Type;

typedef struct {
  Button button1;
  Button button2;
  int score;
  Type type;
  Paddle paddle;
} Player;

void player_init(Player* player, Type type, float starting_x, Button btn1, Button btn2);
void move_player(Player* player, Ball* ball);
