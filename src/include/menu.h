#pragma once

#include "pong.h"

void menu_update(Pong *pong);
void difficulty_update(Pong *pong);
void highscores_update(Pong *pong);
void gameover_update(Pong *pong);


void menu_draw(Display *display);
void difficulty_draw(Display *display);
void highscores_draw(Display *display);
void gameover_draw(Display *display);