#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "ball.h"

void ball_init(Ball* ball) {

  // center of I/O shield screen
  ball->x = MAX_COLS / 2;
  ball->y = MAX_ROWS / 2;

  ball->dx = BALL_VELOCITY;
  ball->dy = 0.0;
}

void ball_set_y_dir(Ball* ball, float y) {
  ball->dy = y;
}

void ball_invert_x_dir(Ball* ball) {
  ball->dx = -ball->dx;
}

void ball_invert_y_dir(Ball* ball) {
  ball->dy = -ball->dy;
}

void ball_update(Ball* ball) {
  ball->x += ball->dx;
  ball->y += ball->dy;
}

// draws following ball to display buffer:
//  *
// ***
//  *
void ball_draw(const Ball* ball, Display* display) {

  const unsigned int page         = (ball->y / 8);
  const unsigned int buffer_bit   = (((int)ball->y) % 8);
  const unsigned int buffer_byte  = (ball->x + (MAX_COLS * page));

  // draw y bits of ball onto display buffer
  if(buffer_bit == 0) {
    const unsigned int upper_byte = buffer_byte - MAX_COLS;
    display->buffer[upper_byte]   |= (0x1 << 7); // set msb
    display->buffer[buffer_byte]  |= 0x3;        // set 2 lsbs
  }
  else if(buffer_bit == 7) {
    const unsigned int lower_byte = buffer_byte + MAX_COLS;
    display->buffer[lower_byte]   |= 0x1;        // set lsb
    display->buffer[buffer_byte]  |= (0x3 << 6); // set 2 msbs
  }
  else {
    display->buffer[buffer_byte] |= (0x7 << (buffer_bit-1)); // set 3 bits
  }

  // draw x bits of ball onto display buffer
  const unsigned int left_byte  = buffer_byte - 1;
  const unsigned int right_byte = buffer_byte + 1;
  display->buffer[left_byte]    |= (0x1 << buffer_bit);
  display->buffer[right_byte]   |= (0x1 << buffer_bit);
}
