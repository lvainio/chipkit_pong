/* This file contains functions related to the OLED display. */

#include <stdint.h> 
#include <pic32mx.h>
#include "display.h"
#include "font.h"

/* Displays a new frame to the OLED display */
void display_frame(const Display *display)
{
    int page;
    for (page = 0; page < MAX_PAGES; page++) {
        spi_select_page(page, 0);

        int col;
        for (col = 0; col < MAX_COLS; col++) {
            spi_send_recv(display->buffer[page * (MAX_COLS) + col]);
        }
    }
}


/* Set all pixels to 0 */
void display_reset(Display *display) 
{
    int i;
    for(i = 0; i < MAX_BYTES; i++) {
        display->buffer[i] = 0;
    }
}


/* Display a string */
void display_string(Display *display, int page, int xpos, char *str) 
{
    if(page < 0 || page > 3) return;
    
    int its = 0;
    while(*str) {
        char c = *str;

        int i;
        for(i = 0; i < 8; i++) {
            display->buffer[page*MAX_COLS + 8*its + xpos + i] |= font[8*c + i];
        }

        str++;
        its++;
    }
}


/* Select which byte to write to on the OLED display */
void spi_select_page(uint8_t page, uint8_t col)
{
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22); // set page command
    spi_send_recv(page); // select which page

    spi_send_recv(col & 0xF);                 // set column (low nibble)
    spi_send_recv(0x10 | ((col >> 4) & 0xF)); // set column (high nibble)

    DISPLAY_CHANGE_TO_DATA_MODE;
}


/* Write a byte to OLED display */
uint8_t spi_send_recv(uint8_t data)
{
    while (!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while (!(SPI2STAT & 1));
    return SPI2BUF;
}


/* Simple delay function */
void quicksleep(int cyc)
{
    int i;
    for (i = cyc; i > 0; i--);
}


/* Performs display on sequence */
void display_init(void)
{
    /* Power to VDD */
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    quicksleep(10);
    DISPLAY_ACTIVATE_VDD;
    quicksleep(1000000);

    spi_send_recv(0xAE);
    DISPLAY_ACTIVATE_RESET;
    quicksleep(10);
    DISPLAY_DO_NOT_RESET;
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_ACTIVATE_VBAT;
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}


