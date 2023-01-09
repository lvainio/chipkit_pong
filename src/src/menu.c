#include <stdint.h>   /* Declarations of uint_32 and the like */
#include "display.h"
#include "menu.h"
#include "input.h"
#include "player.h"

int cursor = 1;

char score[13] = {'S', 'C', 'O', 'R', 'E', ':', ' ', ' ', ' ', '-', ' ', ' ', 0};
char names[8] = {'A', 'A', 'A', '-', 'A', 'A', 'A', 0};

char highscores[100][14];
int num_highscores = 0;

int button3_pressed = 0; 
int button4_pressed = 0; // 0: not pressed, 1: pressed



/* Draw the select cursor onto the display */
void cursor_draw(Display *display) 
{
    display->buffer[ 1 + cursor*MAX_COLS ] |= 0x3c;
    display->buffer[ 2 + cursor*MAX_COLS ] |= 0x18;
}

void cursor_move_up( void ) 
{
    if (cursor == 1) {
            cursor = 3;
    } else {
        cursor--;
    }
}

void cursor_move_down( void ) 
{
    if (cursor == 3) {
            cursor = 1;
    } else {
        cursor++;
    }
}


/* Draw menu to display */
void menu_draw(Display *display)
{
    display_string(display, 0, 5, "MENU");
    display_string(display, 1, 5, "ONE-PLAYER");
    display_string(display, 2, 5, "TWO-PLAYER");
    display_string(display, 3, 5, "HIGHSCORES");

    cursor_draw(display);
}


/* Draw difficulty menu to display */
void difficulty_draw(Display *display) 
{
    display_string(display, 0, 5, "DIFFICULTY");
    display_string(display, 1, 5, "EASY");
    display_string(display, 2, 5, "MEDIUM");
    display_string(display, 3, 5, "HARD");

    cursor_draw(display);
}


/* Draw highscore menu to display */
void highscores_draw(Display *display)
{
    display_string(display, 0, 5, "HIGHSCORES");

    if(num_highscores > 0) display_string(display, 1, 5, highscores[cursor - 1]);
    if(num_highscores > 1) display_string(display, 2, 5, highscores[cursor]);
    if(num_highscores > 2) display_string(display, 3, 5, highscores[cursor + 1]);

}


/* Draw game over screen to display */
void gameover_draw(Display *display)
{
    display_string(display, 0, 5, "GAME OVER");
    display_string(display, 1, 5, score);
    display_string(display, 2, 5, names);
}



/* Update menu */
void menu_update(Pong *pong)
{
    if(button_pressed(BTN_4) && button4_pressed == 0){
        button4_pressed = 1;

        if(cursor == 1) {
            pong->state = Difficulty;

        } else if (cursor == 2) {
            pong->player2.type = HUMAN;
            pong->player2.paddle.speed = HUMAN_SPEED;
            pong->state = Game;
            button3_pressed = 1;

        } else if (cursor == 3) {
            pong->state = Highscores;
        }

        cursor = 1;
        return;

    } else if(!button_pressed(BTN_4) && button4_pressed == 1) {
        button4_pressed = 0;
    }

    if(button_pressed(BTN_1)) cursor_move_up();
    if(button_pressed(BTN_2)) cursor_move_down();
}


void difficulty_update(Pong *pong)
{
    // go back
    if(button_pressed(BTN_3)){
        pong->state = Menu;
        cursor = 1;
        return;
    }

    if(button_pressed(BTN_4) && button4_pressed == 0){
        button4_pressed = 1;

        if(cursor == 1) {
            pong->player2.type = AI_EASY;
             pong->player2.paddle.speed = AI_EASY_SPEED;
            pong->state = Game;

        } else if (cursor == 2) {
            pong->player2.type = AI_MEDIUM;
             pong->player2.paddle.speed = AI_MEDIUM_SPEED;
            pong->state = Game;

        } else if (cursor == 3) {
            pong->player2.type = AI_HARD;
             pong->player2.paddle.speed = AI_HARD_SPEED;
            pong->state = Game;
        }

        button3_pressed = 1;
        cursor = 1;
        return;

    } else if(!button_pressed(BTN_4) && button4_pressed == 1) {
        button4_pressed = 0;
    }


    if(button_pressed(BTN_1)) cursor_move_up();
    if(button_pressed(BTN_2)) cursor_move_down();
}


void highscores_update(Pong *pong) 
{
    // go back
    if(button_pressed(BTN_3)){
        pong->state = Menu;
        cursor = 1;
        return;
    }

    if(button_pressed(BTN_1) && cursor != 1) cursor--;
    if(button_pressed(BTN_2) && cursor < num_highscores - 2) cursor++;

}


void save_highscore( int score1, int score2 ) {
    char hs[13];

    int i;
    for(i = 0; i < 7; i++) {
        hs[i] = names[i];
    }

    hs[7] = ' ';
    hs[8] = (char) score1 / 10 + 48;
    hs[9] = (char) score1 % 10 + 48;
    hs[10] = '-';
    hs[11] = (char) score2 / 10 + 48;
    hs[12] = (char) score2 % 10 + 48;

    hs[13] = 0;

    for(i = 0; i <= 13; i++) {
        highscores[num_highscores][i] = hs[i];
    }
}

void reset_names() {
    int i;
    for(i = 0; i < 3; i++) {
        names[i] = 'A';
        names[4+i] = 'A';
    }
}

void gameover_update(Pong *pong) 
{
    // go back to menu
    if(button_pressed(BTN_3) && button3_pressed == 0){
        pong->state = Menu;
        pong_init(pong);
        reset_names();
        return;
    } else if(!button_pressed(BTN_3) && button3_pressed == 1) {
        button3_pressed = 0;
    }


    // Display the score 
    int score1 = pong->player1.score;
    int score2 = pong->player2.score;

    score[7] = (char) score1 / 10 + 48;
    score[8] = (char) score1 % 10 + 48;

    score[10] = (char) score2 / 10 + 48;
    score[11] = (char) score2 % 10 + 48;


    // Let player enter initials
    static int char_selected = 0;

    if(button_pressed(BTN_1)) {
        if(names[char_selected] == 'A') {
            names[char_selected] = 'Z';
        } else {
            names[char_selected]--;
        }
    } 

    if(button_pressed(BTN_2)) {
        if(names[char_selected] == 'Z') {
            names[char_selected] = 'A';
        } else {
            names[char_selected]++;
        }
    }

    int max_initials = 6;
    Type type = pong->player2.type;
    if(type != HUMAN) {
        max_initials = 2;

        names[4] = 'A';
        names[5] = 'I';

        if(type == AI_EASY)     names[6] = 'E';
        if(type == AI_MEDIUM)   names[6] = 'M';
        if(type == AI_HARD)     names[6] = 'H';
    }

    if(button_pressed(BTN_4) && button4_pressed == 0){
        button4_pressed = 1;

        if(char_selected < max_initials) {
            if(char_selected == 2) char_selected++;
            char_selected ++;
        } else {
            char_selected = 0;
            save_highscore(score1, score2);
            num_highscores++;
            pong->state = Menu;
            pong_init(pong);
            reset_names();
            return;
        }

    } else if(!button_pressed(BTN_4) && button4_pressed == 1) {
        button4_pressed = 0;
    }
}
