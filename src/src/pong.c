#include <stdint.h>
#include "pong.h"
#include "input.h"
#include "font.h"
#include "menu.h"
#include "player.h"
#include "float_comparison.h"

/* Sets pong's was_in_x_bounds_player{x} variables.
 * This is set to true if ball was in x bounds in the previous update.
 * It's used to determine whether an x or y collision was first on a paddle
 * in order to get increased effect on y hits as opposed to x hits. */
bool set_was_in_x_bounds(Pong* pong)
{
  const float ball_x_min = pong->ball.x - 1 + pong->ball.dx;
  const float ball_x_max = pong->ball.x + 1 + pong->ball.dx;

  float paddle_x_min = pong->player1.paddle.x - PADDLE_HALF_WIDTH;
  float paddle_x_max = pong->player1.paddle.x + PADDLE_HALF_WIDTH;

  pong->was_in_x_bounds_player1 =
    (ball_x_min < paddle_x_max && ball_x_max > paddle_x_min);

  paddle_x_min = pong->player2.paddle.x - PADDLE_HALF_WIDTH;
  paddle_x_max = pong->player2.paddle.x + PADDLE_HALF_WIDTH;

  pong->was_in_x_bounds_player2 =
    (ball_x_max > paddle_x_min && ball_x_min < paddle_x_max);
}

/* Initiate a game of pong */
void pong_init(Pong* pong)
{
  ball_init(&pong->ball);
  player_init(&pong->player1, HUMAN, PADDLE_LEFT_STARTING_X, BUTTON_4, BUTTON_3);
  player_init(&pong->player2, HUMAN, PADDLE_RIGHT_STARTING_X, BUTTON_2, BUTTON_1);
  set_was_in_x_bounds(pong);   // initializes was_in_x_bound_player{x}
  display_reset(&pong->display);
  display_frame(&pong->display);
}

/* Resets the ball to the middle of the game screen with new dx and dy */
void reset_ball(Pong* pong)
{
  pong->ball.x  = MAX_COLS / 2;
  pong->ball.y  = MAX_ROWS / 2;

  pong->ball.dx = BALL_VELOCITY;
  pong->ball.dy = 0.0;
}

/* Check if a player has scored a point */
bool is_x_wall_collision(Pong* pong)
{
  if(is_float_leq(pong->ball.x-1+pong->ball.dx, 0) ||
     is_float_geq(pong->ball.x+1+pong->ball.dx, MAX_COLS))
  {
    return true;
  }

  return false;
}

/* Display middle line of game */
void middle_line_draw(Display *display)
{
  int page;
  for(page = 0; page < MAX_PAGES; page++) {
    display->buffer[page*MAX_COLS + MAX_COLS/2 - 1] |= 0x55;
    display->buffer[page*MAX_COLS + MAX_COLS/2] |= 0x55;
  }
}

/* Display in-game score */
void score_draw(Display *display, int score1, int score2)
{
  int digit1 = score1 % 10;
  int digit2 = score2 % 10;
  int tens1  = score1 / 10;
  int tens2  = score2 / 10;

  int i;
  for(i = 0; i < 3; i++) {
    display->buffer[MAX_COLS/2 - 10 + i] |= score_font[tens1*3 + i];
    display->buffer[MAX_COLS/2 - 6 + i]  |= score_font[digit1*3 + i];

    display->buffer[MAX_COLS/2 + 3 + i] |= score_font[tens2*3 + i];
    display->buffer[MAX_COLS/2 + 7 + i] |= score_font[digit2*3 + i];
  }
}

/* Update score if ball collides with an x wall */
void update_player_score(Pong* pong)
{
  if(is_float_leq(pong->ball.x-1+pong->ball.dx, 0))
    pong->player2.score++;

  if(is_float_geq(pong->ball.x+1+pong->ball.dx, MAX_COLS))
    pong->player1.score++;
}

/* return true if game is over */
bool is_game_over(const Pong* pong)
{
  int score1 = pong->player1.score;
  int score2 = pong->player2.score;

  if(score1 == WINNER_SCORE || score2 == WINNER_SCORE) {
    return true;
  }
  return false;
}

/* Handles collisions of ball and long side of screen */
void handle_y_wall_collision(Ball* ball)
{
  const bool is_upper_wall_collision =
    is_float_leq(ball->y-1+ball->dy, 0);

  const bool is_lower_wall_collision =
    is_float_geq(ball->y+1+ball->dy, MAX_ROWS);

  if(is_upper_wall_collision || is_lower_wall_collision)
    ball_invert_y_dir(ball);
}

/* True if collides with top/bottom of player1 paddle */
bool is_player1_y_collision(const Pong* pong)
{
  if(pong->ball.dx > 0)
    return false;

  const float ball_y_min = pong->ball.y - 1 + pong->ball.dy;
  const float ball_y_max = pong->ball.y + 1 + pong->ball.dy;

  const float paddle_y_min = pong->player1.paddle.y - PADDLE_HALF_HEIGHT;
  const float paddle_y_max = pong->player1.paddle.y + PADDLE_HALF_HEIGHT;

  const bool in_y_bounds =
    (ball_y_max > paddle_y_min && ball_y_max < paddle_y_max)
    ||
    (ball_y_min < paddle_y_max && ball_y_min > paddle_y_min);

  return (in_y_bounds && pong->was_in_x_bounds_player1);
}

/* True if ball is within x and y bounds of player1's paddle */
bool is_player1_x_collision(Pong* pong)
{
  if(pong->ball.dx > 0)
    return false;

  const float ball_y_min = pong->ball.y - 1 + pong->ball.dy;
  const float ball_y_max = pong->ball.y + 1 + pong->ball.dy;
  const float ball_x_min = pong->ball.x - 1 + pong->ball.dx;
  const float ball_x_max = pong->ball.x + 1 + pong->ball.dx;

  const float paddle_y_min = pong->player1.paddle.y - PADDLE_HALF_HEIGHT;
  const float paddle_y_max = pong->player1.paddle.y + PADDLE_HALF_HEIGHT;
  const float paddle_x_min = pong->player1.paddle.x - PADDLE_HALF_WIDTH;
  const float paddle_x_max = pong->player1.paddle.x + PADDLE_HALF_WIDTH;

  const bool in_x_bounds =
    (ball_x_min < paddle_x_max && ball_x_max > paddle_x_min);

  const bool in_y_bounds =
    (ball_y_max > paddle_y_min && ball_y_max < paddle_y_max)
    ||
    (ball_y_min < paddle_y_max && ball_y_min > paddle_y_min);

  return (in_y_bounds && in_x_bounds);
}

/* True if collides with top/bottom of player2 paddle */
bool is_player2_y_collision(const Pong* pong)
{
  if(pong->ball.dx < 0)
    return false;

  const float ball_y_min = pong->ball.y - 1 + pong->ball.dy;
  const float ball_y_max = pong->ball.y + 1 + pong->ball.dy;

  const float paddle_y_min = pong->player2.paddle.y - PADDLE_HALF_HEIGHT;
  const float paddle_y_max = pong->player2.paddle.y + PADDLE_HALF_HEIGHT;

  const bool in_y_bounds =
    (ball_y_max > paddle_y_min && ball_y_max < paddle_y_max)
    ||
    (ball_y_min < paddle_y_max && ball_y_min > paddle_y_min);

  return (in_y_bounds && pong->was_in_x_bounds_player2);
}

/* True if ball is within x and y bounds of player2's paddle */
bool is_player2_x_collision(const Pong* pong)
{
  if(pong->ball.dx < 0)
    return false;

  const float ball_y_min = pong->ball.y - 1 + pong->ball.dy;
  const float ball_y_max = pong->ball.y + 1 + pong->ball.dy;
  const float ball_x_min = pong->ball.x - 1 + pong->ball.dx;
  const float ball_x_max = pong->ball.x + 1 + pong->ball.dx;

  const float paddle_y_min = pong->player2.paddle.y - PADDLE_HALF_HEIGHT;
  const float paddle_y_max = pong->player2.paddle.y + PADDLE_HALF_HEIGHT;
  const float paddle_x_min = pong->player2.paddle.x - PADDLE_HALF_WIDTH;
  const float paddle_x_max = pong->player2.paddle.x + PADDLE_HALF_WIDTH;

  const bool in_y_bounds =
    (ball_y_max > paddle_y_min && ball_y_max < paddle_y_max)
    ||
    (ball_y_min < paddle_y_max && ball_y_min > paddle_y_min);

  const bool in_x_bounds =
    (ball_x_max > paddle_x_min && ball_x_min < paddle_x_max);

  return (in_y_bounds && in_x_bounds);
}

/* Updates the ball's direction if there's a collision with a paddle */
void handle_player_collision(Pong* pong)
{
  const float modifier_reduction = 6;

  const bool is_p1_x_collision = is_player1_x_collision(pong);
  const bool is_p2_x_collision = is_player2_x_collision(pong);

  // hits top or bottom of player1 paddle
  if(is_player1_y_collision(pong) && !is_p1_x_collision) {

    const float modifier = (pong->ball.y - pong->player1.paddle.y) / modifier_reduction;
    ball_invert_x_dir(&pong->ball);
    ball_invert_y_dir(&pong->ball);
    ball_set_y_dir(&pong->ball, pong->ball.dy + modifier);
  }
  // hits player 1 paddle on the vertical side
  else if(is_p1_x_collision) {

    const float modifier  = (pong->ball.y - pong->player1.paddle.y) / modifier_reduction;
    const float new_y_dir = (pong->ball.dy + modifier);

    ball_invert_x_dir(&pong->ball);
    ball_set_y_dir(&pong->ball, new_y_dir);
  }

  // hits top or bottom of player2 paddle
  if(is_player2_y_collision(pong) && !is_p2_x_collision) {

    const float modifier = (pong->ball.y - pong->player2.paddle.y) / modifier_reduction;
    ball_invert_x_dir(&pong->ball);
    ball_invert_y_dir(&pong->ball);
    ball_set_y_dir(&pong->ball, pong->ball.dy + modifier);
  }
  // hits player 2 paddle on the vertical side
  else if(is_p2_x_collision) {

    const float modifier  = (pong->ball.y - pong->player2.paddle.y) / modifier_reduction;
    const float new_y_dir = (pong->ball.dy + modifier);

    ball_invert_x_dir(&pong->ball);
    ball_set_y_dir(&pong->ball, new_y_dir);
  }
}

/* Draw to display dependent on the games state */
void pong_draw(Pong* pong)
{
  switch(pong->state) {

    case Menu:        menu_draw(&pong->display);
                      break;

    case Difficulty:  difficulty_draw(&pong->display);
                      break;

    case Highscores:  highscores_draw(&pong->display);
                      break;

    case Game_over:   gameover_draw(&pong->display);
                      break;

    case Game:  {
                  score_draw(&pong->display, pong->player1.score, pong->player2.score);
                  middle_line_draw(&pong->display);

                  ball_draw(&pong->ball, &pong->display);

                  paddle_draw(&pong->player1.paddle, &pong->display);
                  paddle_draw(&pong->player2.paddle, &pong->display);
               }
  }
}

/* Updates pong game dependent on its state */
void pong_update(Pong *pong)
{
  switch(pong->state) {

    case Menu:        menu_update(pong);
                      break;

    case Difficulty:  difficulty_update(pong);
                      break;

    case Highscores:  highscores_update(pong);
                      break;

    case Game_over:   gameover_update(pong);
                      break;

    case Game:  {
                  if(is_x_wall_collision(pong)) {
                    update_player_score(pong);
                    reset_ball(pong);
                  }

                  if(is_game_over(pong))
                    pong->state = Game_over;

                  ball_update(&pong->ball);

                  handle_y_wall_collision(&pong->ball);

                  handle_player_collision(pong);

                  // set after handle collision to use value of previous update
                  // used to determine whether x or y collision was first
                  // in order to get increased effect on y collision
                  set_was_in_x_bounds(pong);

                  move_player(&pong->player1, &pong->ball);
                  move_player(&pong->player2, &pong->ball);
                }
  }
}
