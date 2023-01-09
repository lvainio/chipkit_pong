#pragma once

#include "display.h"

#define PADDLE_WIDTH            2
#define PADDLE_HEIGHT           11
#define PADDLE_HALF_WIDTH       PADDLE_WIDTH / 2
#define PADDLE_HALF_HEIGHT      PADDLE_HEIGHT / 2
#define PADDLE_STARTING_Y       16
#define PADDLE_LEFT_STARTING_X  6
#define PADDLE_RIGHT_STARTING_X 120

typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
} Paddle;

void paddle_init(Paddle* paddle, float starting_x, float paddle_speed);
void paddle_move_up(Paddle* paddle);
void paddle_move_down(Paddle* paddle);
void paddle_draw(const Paddle* paddle, Display* display);


