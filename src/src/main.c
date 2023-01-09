#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdint.h>
#include "pong.h"


Pong pong;

/* Enable interrupts globally */
void enable_interrupt( void )
{
	asm volatile("ei");
	return;
}


/* setup timer2 to throw interrupts 30 timer per second */
void setup_fps_timer( void )
{
  T3CON = 0;          // reset timer
  T3CON |= 0x70;      // 1:256 prescale factor
  PR3 = 15000;        // timer value
  TMR3 = 0;           // reset timer count

  IEC(0) |= (1<<12);  // enable interrupts for timer 3
  IPC(3) |= (7<<2);   // set priority

  T3CON |= (1<<15);   // start timer
}


/* Handle timer interrupts */
void user_isr( void )
{
  // if timer interrupt flag is set
	if(IFS(0) & (1<<12)) {

    IFS(0) &= ~(1<<12); // reset interrupt flag

		pong_update(&pong);

		display_reset(&pong.display);
		pong_draw(&pong);             // draw to logical buffer
		display_frame(&pong.display); // write logical buffer to hardware display
  }

	return;
}


/* Contains main gameloop */
int main(void)
{
	// I/O
	display_init();
	set_buttons_to_input();

	// initiate game
 	pong_init(&pong);

	// timers
	enable_interrupt();
	setup_fps_timer();

	while( 1 ) {
		;
	}
	return 0;
}
