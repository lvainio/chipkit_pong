/* Header file for ball structure */

#pragma once

#include "display.h"

#include <stdbool.h>

#define BALL_VELOCITY -2.5

typedef struct {
  float x, y;
  float dx, dy;
} Ball;

void ball_init(Ball* ball);
void ball_set_y_dir(Ball* ball, float y);
void ball_invert_x_dir(Ball* ball);
void ball_invert_y_dir(Ball* ball);
void ball_update(Ball* ball);
void ball_draw(const Ball* ball, Display* display);
