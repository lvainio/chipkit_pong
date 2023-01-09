/* Header file for pong game */

#pragma once

#include <stdbool.h>

#include "ball.h"
#include "display.h"
#include "player.h"

#define WINNER_SCORE 1

typedef enum {
	Menu,
	Difficulty,
	Highscores,
	Game,
	Game_over
} State;

typedef struct {
    State state;
    Ball ball;
    bool was_in_x_bounds_player1; // used to track if ball was within x bounds
    bool was_in_x_bounds_player2; // of paddle in the previous update of the game
    Player player1;
    Player player2;
    Display display;
} Pong;

void pong_update(Pong *pong);
void pong_init(Pong* pong);
void pong_draw(Pong* pong);
