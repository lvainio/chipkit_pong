#include <stdint.h>
#include "paddle.h"
#include "display.h"


void paddle_init(Paddle* paddle, float starting_x, float paddle_speed) {

  paddle->x       = starting_x;
  paddle->y       = PADDLE_STARTING_Y;
  paddle->width   = PADDLE_WIDTH;
  paddle->height  = PADDLE_HEIGHT;
  paddle->speed   = paddle_speed;
}


void paddle_move_up(Paddle* paddle) {

  // exceeds boundary
  if((paddle->y - PADDLE_HALF_HEIGHT - paddle->speed) <= 0)
    paddle->y = 0 + PADDLE_HALF_HEIGHT;
  else
    paddle->y -= paddle->speed;
}

void paddle_move_down(Paddle* paddle) {

  // exceeds boundary
  if((paddle->y + PADDLE_HALF_HEIGHT + paddle->speed) >= MAX_ROWS)
    paddle->y = MAX_ROWS - PADDLE_HALF_HEIGHT;
  else
    paddle->y += paddle->speed;
}


void paddle_draw(const Paddle* paddle, Display* display) {

  // x and y gives top left position of paddle
  const unsigned int x = (paddle->x - PADDLE_HALF_WIDTH);
  const unsigned int y = (paddle->y - PADDLE_HALF_HEIGHT);

  const unsigned int page                 = (y / 8);
  const unsigned int starting_buffer_bit  = (((int)y) % 8);
  const unsigned int starting_buffer_byte = (x + (MAX_COLS * page));

  int row, col;

  // column being drawn
  for(col = 0; col < PADDLE_WIDTH; col++) {

    unsigned int buffer_bit   = starting_buffer_bit;
    unsigned int buffer_byte  = starting_buffer_byte + col;

    // draw rows in column
    for(row = 0; row < PADDLE_HEIGHT; row++) {

      display->buffer[buffer_byte] |= (0x1 << buffer_bit);

      if(buffer_bit++ == 8) {
        buffer_byte += MAX_COLS;
        buffer_bit = 0;
      }
    }
  }
}
