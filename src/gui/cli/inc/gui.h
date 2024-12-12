#ifndef SRC_GUI_CLI_INC_GUI_H
#define SRC_GUI_CLI_INC_GUI_H

#include <ncurses.h>

#include "brickgame_api.h"

typedef struct {
  WINDOW *game_win;
  WINDOW *next_fig_win;
  WINDOW *info_win;
} Windows;

void init_gui();
void init_windows(Windows *windows);
void draw_menu(WINDOW *win);
Windows *getWindows();
void draw_gameover_win(WINDOW *win);
void draw_winner_win(WINDOW *win);
void draw_start_win(WINDOW *win);
void draw_windows(Windows *windows, GameInfo_t *game, int is_game);
void draw_game_win(WINDOW *win, GameInfo_t *game);
void draw_figure_win(WINDOW *win, GameInfo_t *game);
void draw_info_win(WINDOW *win, GameInfo_t *game);
void draw_status(WINDOW *win, int status);

#endif