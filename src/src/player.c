#include <stdint.h>

#include "ball.h"
#include "float_comparison.h"
#include "player.h"


/* Initiate a player */
void player_init(Player* player, Type type, float starting_x, Button btn1, Button btn2)
{
  player->button1 = btn1;
  player->button2 = btn2;
  player->type = type;
  player->score = 0;

  float paddle_speed = 0.0f;
  switch(type) {
    case HUMAN:     paddle_speed = HUMAN_SPEED;
                    break;
    case AI_EASY:   paddle_speed = AI_EASY_SPEED;
                    break;
    case AI_MEDIUM: paddle_speed = AI_MEDIUM_SPEED;
                    break;
    case AI_HARD:   paddle_speed = AI_HARD_SPEED;
                    break;
  }

  paddle_init(&player->paddle, starting_x, paddle_speed);
}



void player_human_move(Player* player, Direction dir)
{
  switch(dir) {
    case UP:   paddle_move_up(&player->paddle);
               break;
    case DOWN: paddle_move_down(&player->paddle);
               break;
  }
}


void player_ai_medium_move(Player* player, const Ball* ball)
{
  const float diff = (ball->y + ball->dy) - player->paddle.y;

  if(are_floats_eq(diff, 0))
    return;

  if(diff < 0)
    paddle_move_up(&player->paddle);
  else
    paddle_move_down(&player->paddle);
}

void player_ai_easy_move(Player* player, const Ball* ball)
{
  if(is_float_leq(ball->dy, 0.5f) &&
     is_float_geq(ball->dy, -0.5f))
  {
    player_ai_medium_move(player, ball);
  }

  if(ball->dy < 0)
    paddle_move_up(&player->paddle);
  else
    paddle_move_down(&player->paddle);
}

void player_ai_hard_move(Player* player, const Ball* ball)
{
  const float ball_y_min = ball->y - 1 + ball->dy;
  const float ball_y_max = ball->y + 1 + ball->dy;
  const float paddle_y_min = player->paddle.y - PADDLE_HALF_HEIGHT;
  const float paddle_y_max = player->paddle.y + PADDLE_HALF_HEIGHT;

  const bool in_y_bounds =
    (ball_y_max > paddle_y_min && ball_y_max < paddle_y_max)
    ||
    (ball_y_min < paddle_y_max && ball_y_min > paddle_y_min);

  if(!in_y_bounds)
    player_ai_medium_move(player, ball);
}


/* Handles player movement dependant on its type */
void move_player(Player* player, Ball* ball)
{
  Direction dir = NONE;

  int btn1 = player->button1;
  int btn2 = player->button2;

  if(button_pressed(player->button1))
    dir = UP;
  if(button_pressed(player->button2))
    dir = DOWN;

  switch(player->type) {
    case HUMAN:     player_human_move(player, dir);
                    break;
    case AI_EASY:   player_ai_easy_move(player, ball);
                    break;
    case AI_MEDIUM: player_ai_medium_move(player, ball);
                    break;
    case AI_HARD:   player_ai_hard_move(player, ball);
                    break;
  }
}

