#ifndef GUI_H
#define GUI_H


#include "objects.h"
#include <ncurses.h>

typedef struct {
    WINDOW *game_win;
    WINDOW *next_fig_win;
    WINDOW *info_win;
} Windows;

void init_gui();
void draw_windows(Windows *windows, GameInfo_t *game);
void init_windows(Windows *windows);
void draw_game_win(WINDOW *win, GameInfo_t *game);
void draw_figure_win(WINDOW *win, GameInfo_t *game);
void draw_info_win(WINDOW *win, GameInfo_t *game);
void draw_status(WINDOW *win, int status);



#endif