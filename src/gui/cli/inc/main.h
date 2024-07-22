#ifndef MAIN_H
#define MAIN_H

#include "game.h"
#include <ncurses.h>
#include <stdlib.h>

void init_gui();
void init_windows();
void draw_game_win(WINDOW *win);
void draw_figure_win(WINDOW *win);
void draw_info_win(WINDOW *win);

#endif