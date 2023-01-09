/* Header file for display.c */

#pragma once

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define MAX_BYTES       512
#define MAX_COLS        128
#define MAX_ROWS        32
#define MAX_PAGES       4

/* Stores an entire frame */
typedef struct {
    uint8_t buffer[MAX_BYTES];
} Display;

/* Display functions */
void display_frame(const Display *display);
void display_reset(Display *display);
void display_init(void);
void display_string(Display *display, int page, int xpos, char *str);

/* Helpers */
void quicksleep(int cyc);
uint8_t spi_send_recv(uint8_t data);
void spi_select_page(uint8_t page, uint8_t col);