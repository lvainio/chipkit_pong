/* This file contains all necessary functions to setup and 
use the buttons and switches on the chipKIT. */


#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "input.h"


/* Checks whether a specific button is pressed. Returns
1 if the button is pressed, 0 otherwise. */
int button_pressed( int btn )
{
    if( (btn == BTN_1) && (PORTF & (1<<1)) ) return 1; 
    if( (btn == BTN_2) && (PORTD & (1<<5)) ) return 1;
    if( (btn == BTN_3) && (PORTD & (1<<6)) ) return 1;
    if( (btn == BTN_4) && (PORTD & (1<<7)) ) return 1;
    
    return 0;
}


/* Set buttons corresponding TRIS-register bit to input */
void set_buttons_to_input( void ) 
{
    TRISFSET = (1<<1);
    TRISDSET = (1<<5);
    TRISDSET = (1<<6);
    TRISDSET = (1<<7);
    return;
}